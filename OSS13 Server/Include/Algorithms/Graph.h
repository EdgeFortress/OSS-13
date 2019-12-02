#pragma once

#include <map>
#include <vector>

#pragma warning(push, 0)
#include <lemon/list_graph.h>
#include <lemon/maps.h>
#include <lemon/connectivity.h>
#pragma warning(pop)

template<typename LogicNode>
class Graph : public lemon::ListGraph
{
public:
	Graph() : 
		rawNodesToLogic(*this),
		edgeToConnection(*this)
	{ }

	Graph(Graph &&) = default;
	Graph &operator=(Graph &&) = default;

	class Connection {
	public:
		Connection() = default;
		Connection(LogicNode first, LogicNode second) {
			if (first > second)
				std::swap(first, second);
			logicNodes = { first, second };
		}

		const std::array<LogicNode, 2> &GetTiles() const {
			return logicNodes;
		}

		bool operator<(const Connection &other) const {
			if (logicNodes[0] < other.logicNodes[0])
				return true;
			if (logicNodes[0] == other.logicNodes[0] && logicNodes[1] < other.logicNodes[1])
				return true;
			return false;
		}

	private:
		std::array<LogicNode, 2> logicNodes;
	};

	void AddNode(LogicNode node) {
		auto rawNode = ListGraph::addNode();

		rawNodesToLogic[rawNode] = node;
		logicNodesToRaw[node] = rawNode;
	}

	void AddConnection(LogicNode first, LogicNode second) {
		auto edge = ListGraph::addEdge(logicNodesToRaw[first], logicNodesToRaw[second]);
		auto connection = Connection(first, second);

		edgeToConnection[edge] = connection;
		connectionToEdge[connection] = edge;
	}

	void CopyIn(const Graph &other) {
		for (auto &[logicNode, rawNode] : other.logicNodesToRaw)
			AddNode(logicNode);

		for (auto &[connection, edge] : other.connectionToEdge)
			AddConnection(connection.GetTiles()[0], connection.GetTiles()[1]);
	}

	auto GetConnectedComponents() const {
		ListGraph::NodeMap<int> componentsMap(*this);
		auto componentsCount = lemon::connectedComponents(*this, componentsMap);

		std::vector<std::unique_ptr<Graph<LogicNode>>> components;
		for (int i = 0; i < componentsCount; i++)
			components.push_back(std::make_unique<Graph<LogicNode>>());

		for (auto &[logicNode, rawNode] : logicNodesToRaw) {
			auto index = componentsMap[rawNode];
			components[index]->AddNode(logicNode);
		}

		for (auto &[connection, edge] : connectionToEdge) {
			lemon::ListGraph::Node rawNode = logicNodesToRaw.at(connection.GetTiles()[0]);
			auto index = componentsMap[rawNode];
			components[index]->AddConnection(connection.GetTiles()[0], connection.GetTiles()[1]);
		}

		return components;
	}

	void EraseNode(LogicNode node) {
		for (auto iter = connectionToEdge.begin(); iter != connectionToEdge.end();) {
			auto &connection = iter->first;

			if (connection.GetTiles()[0] == node || connection.GetTiles()[1] == node)
				iter = connectionToEdge.erase(iter);
			else
				iter++;
		}

		ListGraph::erase(logicNodesToRaw[node]);
		logicNodesToRaw.erase(node);
	}

	void EraseConnection(LogicNode first, LogicNode second) {
		Connection connection{first, second};

		ListGraph::erase(connectionToEdge[connection]);
		connectionToEdge.erase(connection);
	}

	void Clear() {
		ListGraph::clear();
		logicNodesToRaw.clear();
		connectionToEdge.clear();
	}

	bool IsContain(LogicNode node) const {
		return logicNodesToRaw.find(node) != logicNodesToRaw.end();
	}

	size_t Count() const {
		return logicNodesToRaw.size();
	}

	size_t CountConnections() const {
		return connectionToEdge.size();
	}

	bool IsEmpty() const {
		return !lemon::countNodes(*this);
	}


	class const_iterator {
	public:
		const_iterator(typename std::map<LogicNode, lemon::ListGraph::Node>::iterator iter)
			: iter(iter)
		{ }
		const_iterator& operator++() { iter++; return *this; };
		LogicNode operator*() const { return iter->first; }
		bool operator!=(const const_iterator& it) const { return iter != it.iter; }
	protected:
		typename std::map<LogicNode, ListGraph::Node>::iterator iter;
	};

	class iterator : public const_iterator {
	public:
		using const_iterator::const_iterator;

		iterator& operator++() { const_iterator::iter++; return *this; };
		LogicNode &operator*() const { return const_cast<LogicNode &>(const_iterator::iter->first); }
		bool operator!=(const iterator& it) const { return const_iterator::iter != it.iter; }
	};


	iterator begin() { return iterator(logicNodesToRaw.begin()); }
	iterator end() { return iterator(logicNodesToRaw.end()); }
	const_iterator cbegin() const { return const_iterator(logicNodesToRaw.begin()); }
	const_iterator cend() const { return const_iterator(logicNodesToRaw.end()); }

private:
	ListGraph::NodeMap<LogicNode> rawNodesToLogic;
	std::map<LogicNode, lemon::ListGraph::Node> logicNodesToRaw;

	ListGraph::EdgeMap<Connection> edgeToConnection;
	std::map<Connection, ListGraph::Edge> connectionToEdge;
};
