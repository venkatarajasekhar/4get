#include <gtest\gtest.h>
#include "logic_tester.h"


TEST_F(ParserTest, MethodReturnsFirstItem)
{
	vector<string> inputBits1(SLOT_SIZE);
	parser.parseInput(input1,inputBits1);
	EXPECT_EQ("add",parser.getCommand());
}

TEST_F(ParserTest, TestDelete)
{
	vector<string> inputBits2(SLOT_SIZE);
	parser.parseInput(input2, inputBits2);
	EXPECT_EQ("delete",parser.getCommand());
}

TEST_F(ParserTest, TestMark)
{
	vector<string> inputBits3(SLOT_SIZE);
	parser.parseInput(input3, inputBits3);
	EXPECT_EQ("mark",parser.getCommand());
}

TEST_F(ParserTest, TestModify)
{
	vector<string> inputBits4(SLOT_SIZE);
	parser.parseInput(input4, inputBits4);
	EXPECT_EQ("modify",parser.getCommand());
}


TEST_F(LogTest, TestLog)
{
	//this is a test for different types of log.
	logging(inputMessage, type, status);
	logging(inputMessage2, type2, status2);
}
TEST_F(ExecutorTest, TestExecutorAdd){
	ExecutorTester.setListType(listToDo);
	bool result = ExecutorTester.processInput(taskAdd);
	EXPECT_EQ(true,result);
}
//TEST_F(ExecutorTest, TestExecutorDelete){
//	ExecutorTester.setListType(listToDo);
//	bool result = ExecutorTester.stringCollector(taskDelete);
//	EXPECT_EQ(true,result);
//}

TEST_F(TaskListTest, TestListAdd){
	Task* tempTask;
	list<Task*> tempList;
	long long tempID;
	tempList = TaskListTester.getList(listToDo);
	list<Task*>::iterator it = tempList.begin();
	tempTask = (*it);
	tempID = tempTask->getTaskId();
	EXPECT_EQ(tempID,1);
	++it;
	tempTask = (*it);
	tempID = tempTask->getTaskId();
	EXPECT_EQ(tempID,3);
	++it;
	tempTask = (*it);
	tempID = tempTask->getTaskId();
	EXPECT_EQ(tempID,2);
}

TEST_F(TaskListTest, TestListDelete){
	list<Task*> tempList;
	int listSize;
	TaskListTester.setCurrentDisplayed(listToDo);
	tempList = TaskListTester.getList(listToDo);
	listSize = tempList.size();
	EXPECT_EQ(listSize, 3);
	TaskListTester.deleteIndexFromList(1, true);
	tempList = TaskListTester.getList(listToDo);
	listSize = tempList.size();
	EXPECT_EQ(listSize, 2);
	TaskListTester.deleteIndexFromList(1, true);
	tempList = TaskListTester.getList(listToDo);
	listSize = tempList.size();
	EXPECT_EQ(listSize, 1);
	TaskListTester.deleteIndexFromList(1, true);
	tempList = TaskListTester.getList(listToDo);
	listSize = tempList.size();
	EXPECT_EQ(listSize, 0);
}

TEST_F(TaskListTest, TestMarkDone){
	list<Task*> tempToDoList;
	list<Task*> tempCompletedList;
	int toDoSize, completedSize;
	tempToDoList = TaskListTester.getList(listToDo);
	tempCompletedList = TaskListTester.getList(listCompleted);
	TaskListTester.setCurrentDisplayed(listToDo);
	toDoSize = tempToDoList.size();
	completedSize = tempCompletedList.size();
	EXPECT_EQ(toDoSize, 3);
	EXPECT_EQ(completedSize, 0);
	TaskListTester.markDone(1);
	tempToDoList = TaskListTester.getList(listToDo);
	tempCompletedList = TaskListTester.getList(listCompleted);
	toDoSize = tempToDoList.size();
	completedSize = tempCompletedList.size();
	EXPECT_EQ(toDoSize, 2);
	EXPECT_EQ(completedSize, 1);
}