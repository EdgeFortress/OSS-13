#include <Shared/Network/Syncable.h>

#include <gtest/gtest.h>

DEFINE_SYNCABLE(TestSyncable)
	uf::SyncField<int> a;
	uf::SyncField<std::string> b;

	TestSyncable() {
		DECLARE_SYNCABLE_FIELD(&TestSyncable::a);
		DECLARE_SYNCABLE_FIELD(&TestSyncable::b);
	}
DEFINE_SYNCABLE_END

TEST(Syncable, test) {
	TestSyncable testSyncable;
	testSyncable.a = 12345;
	testSyncable.b = "test string";

	auto changes = testSyncable.GetChanges();

	TestSyncable otherSyncable;
	otherSyncable.AmendChanges(std::move(changes));

	CHECK(testSyncable.a.GetValue() == otherSyncable.a.GetValue());
	CHECK(testSyncable.b.GetValue() == otherSyncable.b.GetValue());
}
