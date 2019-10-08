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

TEST(Syncable, IsChangedReturnsTrueAfterFieldChange) {
	TestSyncable testSyncable;
	testSyncable.a = 12345;

	CHECK(testSyncable.IsChanged());
}

TEST(Syncable, CorrectValuesAfterAmend) {
	TestSyncable testSyncable;
	testSyncable.a = 12345;
	testSyncable.b = "test string";

	auto changes = testSyncable.PopChanges();

	TestSyncable otherSyncable;
	otherSyncable.AmendChanges(std::move(changes));

	CHECK(testSyncable.a.GetValue() == otherSyncable.a.GetValue());
	CHECK(testSyncable.b.GetValue() == otherSyncable.b.GetValue());
}

TEST(Syncable, AmendDefaultValues) {
	TestSyncable testSyncable;

	auto changes = testSyncable.PopChanges();

	TestSyncable otherSyncable;
	otherSyncable.AmendChanges(std::move(changes));

	CHECK(testSyncable.a.GetValue() == otherSyncable.a.GetValue());
	CHECK(testSyncable.b.GetValue() == otherSyncable.b.GetValue());
}
