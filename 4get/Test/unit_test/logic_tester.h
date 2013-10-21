#include <gtest\gtest.h>
#include <gtest\gtest.h>
#include "logic_parser.h"
#include "logic_task.h"
#include "logic_task_deadline.h"
#include "logic_task_floating.h"
#include "logic_task_timed.h"
#include "log.h"
#include "logic_executor.h"

//#include "logic_task_list.h"
class StorageTest : public ::testing::Test {
protected:
	StorageTest() {
		// You can do set-up work for each test here.
	}

	virtual ~StorageTest() {
		// You can do clean-up work that doesn't throw exceptions here.
	}

	// If the constructor and destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods:

	virtual void SetUp() {
		// Code here will be called immediately after the constructor (right
		// before each test).

	}

	virtual void TearDown() {
		// Code here will be called immediately after each test (right
		// before the destructor).
	}

	// Objects declared here can be used by all tests in the test case for Foo.
};


class ParserTest : public ::testing::Test {
protected:
	// You can remove any or all of the following functions if its body
	// is empty.

	Parser parser;
	string input1, input2, input3, input4;

	int id;
	string description;
	string location;
	time_t reminderTime;
	Priority priority;
	Status status;
	RepeatType repeat;
	time_t startTime;
	time_t endTime;

	ParserTest() {
		// You can do set-up work for each test here.
	}

	virtual ~ParserTest() {
		// You can do clean-up work that doesn't throw exceptions here.
	}

	// If the constructor and destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods:

	virtual void SetUp() {
		// Code here will be called immediately after the constructor (right
		// before each test).
		input1 = "Add HomeWork from EE2020 ,at com1 ,repeat weekly ,From Monday 2PM to Saturday 4PM ,!";
		input2 = "delete 2";
		input3 = "mark 3 Undone";
		input4 = "modify 2 ,from 2000 to 1900 ,at com2 ,Repeat Weekly ,remind on Tuesday 2000 ,! incomplete";






		id = 1;
		description = "HomeWork from EE2020";
		location = "com1";




		priority = high;
		status = incomplete;
		repeat = repeatNone;





	}

	virtual void TearDown() {
		// Code here will be called immediately after each test (right
		// before the destructor).
	}

	// Objects declared here can be used by all tests in the test case for Foo.
};

class LogTest : public ::testing::Test{
protected:

	string inputMessage, inputMessage2;
	LogType type, type2;
	LogStatus status, status2;

	LogTest() {
		// You can do set-up work for each test here.

	}

	virtual ~LogTest() {
		// You can do clean-up work that doesn't throw exceptions here.
	}

	// If the constructor and destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods:

	virtual void SetUp() {
		// Code here will be called immediately after the constructor (right
		// before each test).
		inputMessage = "testing";
		type = Info;
		status = None;

		inputMessage2 = "PASS PLS!!";
		type2 = Debug;
		status2 = Pass;

	}

	virtual void TearDown() {
		// Code here will be called immediately after each test (right
		// before the destructor).
	}

	// Objects declared here can be used by all tests in the test case for Foo.
};
class ExecutorTest : public ::testing::Test{
protected:

	Executor ExecutorTester;
	string taskAdd;
	
	

	ExecutorTest() {
		// You can do set-up work for each test here.

	}

	virtual ~ExecutorTest() {
		// You can do clean-up work that doesn't throw exceptions here.
	}


	// If the constructor and destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods:

	virtual void SetUp() {
		// Code here will be called immediately after the constructor (right
		// before each test).
		taskAdd = "add hello";
		
	}

	virtual void TearDown() {
		// Code here will be called immediately after each test (right
		// before the destructor).
	}

	// Objects declared here can be used by all tests in the test case for Foo.
};