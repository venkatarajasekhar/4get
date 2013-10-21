#include "logic_executor.h"

const int Executor::CONSTANT_MULTIPLIER_YEAR = 1000000;
const int Executor::CONSTANT_MULTIPLIER_MONTH = 10000;
const int Executor::CONSTANT_MULTIPLIER_DAY = 100;
const int Executor::CONSTANT_MONTH_ONE = 1;

Executor::Executor(){
	taskID = retrieveCurrentDate();
}
bool Executor::stringCollector(string task){
	vector<string> vectorOfInputs(SLOT_SIZE);
	try{
		parser.parseInput(task, (vectorOfInputs));
		if(receive(vectorOfInputs[SLOT_COMMAND], vectorOfInputs))
		{
			//logging("Number of times UI call stringColletor", Info, Pass);
			return true;
		}
		else
			return false;
	} catch(string Error){
		throw;
	}
}
bool Executor::receive(string usercommand, vector<string> vectorOfInputs){
	Command commandType = determineCommandType(usercommand);
	storeCommands(commandType);          
	switch(commandType)
	{
	case commandAdd:
		return adderFunction(vectorOfInputs);
	case commandDelete:
		return deleteFunction(vectorOfInputs);
	case commandMark:
		return markFunction(vectorOfInputs);
	case commandModify:
		return modifyFunction(vectorOfInputs);
	case commandUndo:
		return undoFunction();
	case commandRedo:
		return redoFunction();
	default: return false;
	}
}
Enum::Command Executor::determineCommandType (string commandTypeString){
	if(isEqual(commandTypeString, COMMAND_ADD))
		return Command::commandAdd;
	else if(isEqual(commandTypeString, COMMAND_DELETE))
		return Command::commandDelete;
	else if(isEqual(commandTypeString, COMMAND_MARK))
		return Command::commandMark;
	else if(isEqual(commandTypeString, COMMAND_MODIFY))
		return Command::commandModify;
	else if(isEqual(commandTypeString, COMMAND_UNDO))
		return Command::commandUndo;
	else if(isEqual(commandTypeString, COMMAND_REDO))
		return Command::commandRedo;
	else
		return Command::commandInvalid;
}
list<Task*> Executor::getUpdatedList(ListType listType){
	return taskList.obtainList(listType);
}
bool Executor::adderFunction(vector<string> vectorOfInputs){
	long long id;
	string description, location;
	time_t reminderTime, startTime, endTime;
	RepeatType repeat;
	Priority priority;
	Status status = statusNone;
	TaskType taskTypeToCreate;

	id = taskID;
	setParameters(description,
		location,
		priority,
		repeat,
		startTime,
		endTime,
		reminderTime,
		taskTypeToCreate,
		vectorOfInputs);

	if(taskTypeToCreate == floating){
		TaskFloating newTask(id,
			description,
			location,
			reminderTime,
			priority,
			status);

		taskGlobal = new TaskFloating;
		*taskGlobal = newTask;
	}

	else if(taskTypeToCreate == deadline){
		TaskDeadline newTask(id,
			description, 
			location, 
			reminderTime, 
			priority, 
			status, 
			repeat, 
			endTime); 
		cout << "deadline task" <<endl;
		taskGlobal = new TaskDeadline;
		*taskGlobal = newTask;
	}

	else if(taskTypeToCreate == timed){
		TaskTimed newTask(id, 
			description, 
			location, 
			reminderTime, 
			priority, 
			status, 
			repeat, 
			startTime, 
			endTime);

		taskGlobal = new TaskTimed;
		*taskGlobal = newTask;
	}
	else{
		throw string(Message::MESSAGE_ERROR_COMMAND_ADD);
	}

	storeTask(*taskGlobal);
	taskList.addToList(taskGlobal, listType);
	return true;
}
bool Executor::deleteFunction(vector<string> vectorOfInputs){
	int deleteNumber;
	deleteNumber = convert.convertStringToInt(vectorOfInputs[SLOT_SLOT_NUMBER]);
	try{
		storeTask(*taskList.obtainTask(deleteNumber));
		taskList.deleteFromList(deleteNumber, true);
	} catch (string errorStr){
		throw;
	}
	return true;
}
bool Executor::markFunction(vector<string> vectorOfInputs){
	int markNumber;
	markNumber = convert.convertStringToInt(vectorOfInputs[SLOT_SLOT_NUMBER]);
	try{
		storeTask(*taskList.obtainTask(markNumber));
		taskList.markDone(markNumber);
	}catch (string errorStr){
		throw;
	}
	return true;
}
bool Executor::modifyFunction(vector<string> vectorOfInputs){
	Task* taskTemp;
	Task* taskNew;
	Task taskModified;
	long long id;
	int modifyNumber;
	string description, 
		location;
	time_t reminderTime, 
		startTime, 
		endTime;
	Priority priority;
	RepeatType repeat;
	TaskType typeOfTask, typeOfOldTask;

	modifyNumber = convert.convertStringToInt(vectorOfInputs[SLOT_SLOT_NUMBER]);
	taskTemp = taskList.obtainTask(modifyNumber);
	typeOfOldTask = taskTemp->getTaskType();
	storeTask(*taskTemp);
	setParameters(description,
		location,
		priority,
		repeat,
		startTime,
		endTime,
		reminderTime,
		typeOfTask,
		vectorOfInputs);

	bool isNoEndTime = (endTime == 0);
	bool isNoStartTime = (startTime == 0);
	if(!description.empty()){
		taskTemp->setTaskDescription(description);
	}
	if(!location.empty()){
		taskTemp->setTaskLocation(location);
	}
	if(!vectorOfInputs[SLOT_REMIND_TIME].empty())
	{
		taskTemp->setTaskReminder(reminderTime);
	}
	if(!vectorOfInputs[SLOT_START_TIME].empty() || !vectorOfInputs[SLOT_START_DATE].empty()){
		taskTemp->setTaskStart(startTime);
	}
	//change floating to deadline task
	if(typeOfOldTask == floating && !isNoEndTime && isNoStartTime){
		id = taskTemp->getTaskId();									//set another task
		taskTemp->setTaskEnd(endTime);
		string description = taskTemp->getTaskDescription();
		string location = taskTemp->getTaskLocation();
		Priority priority = taskTemp->getTaskPriority();
		Status status = taskTemp->getTaskStatus();
		RepeatType repeat = taskTemp->getTaskRepeat();
		time_t reminderTime = taskTemp->getTaskReminder();
		taskNew = new TaskDeadline(id,
			description, 
			location, 
			reminderTime, 
			priority, 
			status, 
			repeat, 
			endTime);
		try{
			taskList.deleteIDFromList(id, listType,true);
			taskList.addToList(taskNew, listType);
		} catch(string error){
			throw;
		}
	}
	//change floating to timed task
	if(typeOfOldTask == floating && !isNoEndTime && !isNoStartTime){
		taskTemp->setTaskEnd(endTime);
		taskTemp->setTaskStart(startTime);
		id = taskTemp->getTaskId();
		string description = taskTemp->getTaskDescription();
		string location = taskTemp->getTaskLocation();
		Priority priority = taskTemp->getTaskPriority();
		Status status = taskTemp->getTaskStatus();
		RepeatType repeat = taskTemp->getTaskRepeat();
		time_t reminderTime = taskTemp->getTaskReminder();
		taskNew = new TaskTimed(id, 
			description, 
			location, 
			reminderTime, 
			priority, 
			status, 
			repeat, 
			startTime, 
			endTime);
		try{
			taskList.deleteIDFromList(id, listType, true);
			taskList.addToList(taskNew, listType);
		} catch(string error){
			throw;
		}
	}
	//change deadline task to timed
	if(typeOfOldTask == deadline && !isNoStartTime){
		taskTemp->setTaskStart(startTime);
		id = taskTemp->getTaskId();
		string description = taskTemp->getTaskDescription();
		string location = taskTemp->getTaskLocation();
		Priority priority = taskTemp->getTaskPriority();
		Status status = taskTemp->getTaskStatus();
		RepeatType repeat = taskTemp->getTaskRepeat();
		time_t reminderTime = taskTemp->getTaskReminder();		
		taskNew = new TaskTimed(id, 
			description, 
			location, 
			reminderTime, 
			priority, 
			status, 
			repeat, 
			startTime, 
			endTime);
		try{
			taskList.deleteIDFromList(id, listType, true);
			taskList.addToList(taskNew, listType);
		} catch(string error){
			throw;
		}
	}
	redoModifiedTaskStack.push(*taskTemp);
	return true;
}
bool Executor::undoFunction(){
	Task taskTemp;
	Command commandType;

	undoCommandStack.pop();
	if(undoCommandStack.empty()){
		return false;
	}

	commandType = undoCommandStack.top();
	taskTemp = undoTaskStack.top();
	redoCommandStack.push(commandType);
	redoTaskStack.push(taskTemp);

	switch(commandType)
	{
	case commandAdd: 
		taskList.deleteIDFromList(taskTemp.getTaskId(), listType, true);
		break;
	case commandDelete:
		taskList.addToList(&taskTemp, listType);
		break;
	case commandMark:
		taskList.deleteIDFromList(taskTemp.getTaskId(), listCompleted, true);
		taskList.addToList(&taskTemp, listToDo);
		break;
	case commandModify:
		taskList.deleteIDFromList(taskTemp.getTaskId(), listType, true);
		taskList.addToList(&taskTemp, listType);
		break;
	default:
		return false;
	}
	undoCommandStack.pop();
	undoTaskStack.pop();

	return true;
}
bool Executor::redoFunction(){
	Task taskTemp;
	Command commandType;

	if(redoCommandStack.empty()){
		return false;
	}

	commandType = redoCommandStack.top();
	switch(commandType)
	{
	case commandAdd:{
		taskTemp = redoTaskStack.top();
		taskList.addToList(&taskTemp, listType);
		break;
					}
	case commandDelete:{
		taskTemp = redoTaskStack.top();
		taskList.deleteIDFromList(taskTemp.getTaskId(), listType, true);
		break;
					   }
	case commandMark:{
		taskTemp = redoTaskStack.top();
		taskList.addToList(&taskTemp, listCompleted);
		taskList.deleteIDFromList(taskTemp.getTaskId(), listToDo, true);
		break;
					 }
	case commandModify:{
		taskTemp = redoModifiedTaskStack.top();
		taskList.addToList(&taskTemp, listType);
		taskList.deleteIDFromList(taskTemp.getTaskId(), listType, true);
		redoModifiedTaskStack.pop();
		break;
					   }
	default:
		throw string(MESSAGE_ERROR_NOTHING_TO_REDO);
	}
	redoCommandStack.pop();
	redoTaskStack.pop();
	return true;
}
bool Executor::isEqual(string str1, const string str2){
	if(str1 == str2){
		return true;
	}
	return false;
}
long long Executor::retrieveCurrentDate(){
	/*time_t timeTemp;
	time_t timeTemp;
	long long yearMonthDay;
	tm* currentTime;
	*/
	//time(&timeTemp);
	//currentTime = localtime(&timeTemp);

	//long long day = currentTime ->tm_mday;
	//long long month = CONSTANT_MONTH_ONE + currentTime ->tm_mon;
	//long long year = currentTime ->tm_year + CONSTANT_START_YEAR;

	//day = day*CONSTANT_MULTIPLIER_DAY;
	//month = month*CONSTANT_MULTIPLIER_MONTH;
	//year = year*CONSTANT_MULTIPLIER_YEAR;
	//yearMonthDay = (year + month + day);

	//return yearMonthDay;
	time_t timeTemp;
	long long yearMonthDay;

	return yearMonthDay = time(&timeTemp);
}
bool Executor::storeTask(Task taskTemp){
	undoTaskStack.push(taskTemp);
	return true;
}
bool Executor::storeCommands(Command command){
	undoCommandStack.push(command);
	return true;
}
bool Executor::setListType(ListType uiListType)
{
	listType = uiListType;
	return true;
}
bool Executor::setParameters(string &description,
							 string &location,
							 Priority &priority,
							 RepeatType &repeat,
							 time_t &startTime,
							 time_t &endTime,
							 time_t &reminderTime,
							 TaskType &typeOfTask,
							 vector<string> &vectorOfInputs){
								 string descriptionSlot,
									 locationSlot,
									 prioritySlot,
									 repeatSlot,
									 startDateSlot,
									 startTimeSlot, 
									 endDateSlot, 
									 endTimeSlot, 
									 reminderDateSlot, 
									 reminderTimeSlot;

								 descriptionSlot = vectorOfInputs[SLOT_DESCRIPTION];
								 locationSlot = vectorOfInputs[SLOT_LOCATION];
								 prioritySlot = vectorOfInputs[SLOT_PRIORITY];
								 repeatSlot = vectorOfInputs[SLOT_REPEAT];
								 startDateSlot = vectorOfInputs[SLOT_START_DATE];
								 startTimeSlot = vectorOfInputs[SLOT_START_TIME];
								 endDateSlot = vectorOfInputs[SLOT_END_DATE];
								 endTimeSlot = vectorOfInputs[SLOT_END_TIME];
								 reminderDateSlot = vectorOfInputs[SLOT_REMIND_DATE];
								 reminderTimeSlot = vectorOfInputs[SLOT_REMIND_TIME];
								 description = descriptionSlot;
								 location = locationSlot;
								 priority = convert.convertStringToPriority(prioritySlot);
								 repeat = convert.convertStringToRepeatType(repeatSlot);
								 reminderTime = convert.convertStringToTime(reminderDateSlot, reminderTimeSlot);
								 typeOfTask = convert.convertStringToTime(startDateSlot, 
									 startTimeSlot, 
									 endDateSlot, 
									 endTimeSlot, 
									 startTime, 
									 endTime);
								 return true;
}
void Executor::empty_task(){
	assert(taskGlobal != NULL);
}