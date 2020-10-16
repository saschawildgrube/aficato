<?php
	
	require_once(GetWDKDir()."wdk_eventdispatcher.inc");
	
	class CDummyEventHandler implements IEventHandler
	{
		public $m_strID;
		
		function __construct ($strID)
		{
			$this->m_strID = $strID;	
		}
		function EventHandler($strEvent,$arrayParams)	
		{
			$test = $arrayParams["test"];
			$test->Trace("EventHandler($strEvent,arrayParams)");
			$strKey = $this->m_strID . $strEvent;
			$test->m_arrayEventCounter[$strKey] = intval(ArrayGetValue($test->m_arrayEventCounter,$strKey)) + 1;
			$test->Trace("EventCounter[$strKey] = ".$test->m_arrayEventCounter[$strKey]);
		}
	}
	
	class CTest extends CUnitTest
	{
		public $m_arrayEventCounter;
		
		function __construct()
		{
			parent::__construct("TriggerEvent");
		}

		
		function TestCase_TriggerEvents(
			$dispatcher,
			$arrayEventsToTrigger,
			$arrayExpectedResult)
		{ 
			$this->Trace("TestCase_TriggerEvents");
	
			$this->Trace("arrayExpectedResult");
			$this->Trace($arrayExpectedResult);
	

			$this->m_arrayEventCounter = array();
			foreach ($arrayEventsToTrigger as $strEvent)
			{
				$dispatcher->TriggerEvent($strEvent,array("test" => $this));
			}
				
			$this->Trace("this->m_arrayEventCounter");
			$this->Trace($this->m_arrayEventCounter);
	
			if ($this->m_arrayEventCounter == $arrayExpectedResult)
			{
				$this->Trace("Testcase PASSED!");
			}
			else
			{
				$this->Trace("Testcase FAILED!");	
				$this->SetResult(false);	
			}
			$this->Trace("");
			$this->Trace("");
		}



		function CallbackTest()
		{
			parent::CallbackTest();
					
			$this->SetResult(true);
			
			
			
			$dispatcher = new CEventDispatcher();			
			$handler1 = new CDummyEventHandler("1");
			$handler2 = new CDummyEventHandler("2");
			$dispatcher->RegisterEvent("test",$handler1);
			$dispatcher->RegisterEvent("test",$handler2);
			$arrayEvents = array("test");
			$arrayExceptedResult = array("1test" => 1, "2test" => 1);
			$this->TestCase_TriggerEvents($dispatcher,$arrayEvents,$arrayExceptedResult);

			
			$dispatcher = new CEventDispatcher();			
			$handler1 = new CDummyEventHandler("1");
			$handler2 = new CDummyEventHandler("2");
			$handler3 = new CDummyEventHandler("3");
			$dispatcher->RegisterEvent("test",$handler1);
			$dispatcher->RegisterEvent("test",$handler2);
			$dispatcher->RegisterEvent("test",$handler3);
			$arrayEvents = array("test","test");
			$arrayExceptedResult = array("1test" => 2,"2test" => 2,"3test" => 2);
			$this->TestCase_TriggerEvents($dispatcher,$arrayEvents,$arrayExceptedResult);


			$dispatcher = new CEventDispatcher();			
			$handler1 = new CDummyEventHandler("1");
			$handler2 = new CDummyEventHandler("2");
			$handler3 = new CDummyEventHandler("3");
			$dispatcher->RegisterEvent("test",$handler1);
			$dispatcher->RegisterEvent("start",$handler2);
			$dispatcher->RegisterEvent("end",$handler3);
			$arrayEvents = array("test","start","end");
			$arrayExceptedResult = array("1test" => 1,"2start" => 1,"3end" => 1);
			$this->TestCase_TriggerEvents($dispatcher,$arrayEvents,$arrayExceptedResult);


/*
			$dispatcher = new CEventDispatcher();			
			$handler1 = new CDummyEventHandler("1");
			$handler2 = new CDummyEventHandler("2");
			$handler3 = new CDummyEventHandler("3");
			$dispatcher->RegisterEvent("test",$handler1);
			$dispatcher->RegisterEvent("start",$handler2);
			$dispatcher->RegisterEvent("end",$handler3);
			$dispatcher->UnregisterEvent("test",$handler1);
			$arrayEvents = array("test","start","end");
			$arrayExceptedResult = array("2start" => 1,"3end" => 1);
			$this->TestCase_TriggerEvents($dispatcher,$arrayEvents,$arrayExceptedResult);
*/



		}
		
		
	}
	
	

		
