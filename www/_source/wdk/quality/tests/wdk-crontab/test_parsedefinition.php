<?php

	require_once(GetWDKDir()."wdk_crontab.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test CrontabCalculateNextDateTime");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->Trace("Do some init tasks here!");
			return true;
		}


		function TestCase_CrontabParseDefinition($strCrontabDefinition,$nMin,$nMax,$arrayExpectedResult)
		{
			$this->Trace("TestCase_CrontabParseDefinition");
			$this->Trace("strCrontabDefintion = \"$strCrontabDefinition\", nMin = $nMin, nMax = $nMax");
			$this->Trace("arrayExpectedResult:");
			if ($arrayExpectedResult == false)
			{
				$this->Trace("FALSE");
			}
			else
			{
				$this->Trace($arrayExpectedResult);
			}
					
			$arrayResult = CrontabParseDefinition($strCrontabDefinition,$nMin,$nMax);
				
			$this->Trace("Testcase CrontabParseDefintion returns:");
			if ($arrayResult == false)
			{
				$this->Trace("FALSE");
			}
			else
			{
				$this->Trace($arrayResult);
			}
			
			if ($arrayResult == $arrayExpectedResult)
			{
				$this->Trace("Testcase PASSED!");
			}
			else
			{
				$this->Trace("Testcase FAILED!");	
				$this->SetResult(false);
			}
			$this->Trace("");
		}


		
		function CallbackTest()
		{
			parent::CallbackTest();
			$this->SetResult(true);	

			$this->TestCase_CrontabParseDefinition("",0,10,false);
			$this->TestCase_CrontabParseDefinition("BOGUS",0,10,false);
			$this->TestCase_CrontabParseDefinition("",1,10,false);
			$this->TestCase_CrontabParseDefinition("BOGUS",1,10,false);

			
			$this->TestCase_CrontabParseDefinition("*",0,10,array(0,1,2,3,4,5,6,7,8,9,10));
			
			$arrayExpected = array();
			for ($nValue = 0; $nValue < 60; $nValue++)
			{
				array_push($arrayExpected,$nValue);
			}
			$this->TestCase_CrontabParseDefinition("*",0,59,$arrayExpected);
				
	
			$this->TestCase_CrontabParseDefinition("10-15",0,59,array(10,11,12,13,14,15));
	
	
			$arrayExpected = array();
			for ($nValue = 0; $nValue < 60; $nValue+=5)
			{
				array_push($arrayExpected,$nValue);
			}
			$this->TestCase_CrontabParseDefinition("*/5",0,59,$arrayExpected);
			
	
			$arrayExpected = array();
			for ($nValue = 22; $nValue <= 59; $nValue+=2)
			{
				array_push($arrayExpected,$nValue);
			}
			$this->TestCase_CrontabParseDefinition("22-59/2",0,59,$arrayExpected);
	
	
			$arrayExpected = array();
			for ($nValue = 10; $nValue <= 15; $nValue+=2)
			{
				array_push($arrayExpected,$nValue);
			}
			for ($nValue = 40; $nValue <= 55 ; $nValue+=5)
			{
				array_push($arrayExpected,$nValue);
			}
			$this->TestCase_CrontabParseDefinition("10-15/2,40-55/5",0,59,$arrayExpected);
	
	
			$arrayExpected = array();
			for ($nValue = 0; $nValue <= 59; $nValue+=15)
			{
				array_push($arrayExpected,$nValue);
			}
			for ($nValue = 30; $nValue <= 40 ; $nValue+=2)
			{
				array_push($arrayExpected,$nValue);
			}
			$arrayExpected = array_unique($arrayExpected);
			sort($arrayExpected);
			$this->TestCase_CrontabParseDefinition("30-40/2,*/15",0,59,$arrayExpected);


		}
		
		function CallbackCleanup()
		{
			parent::CallbackCleanup();
			return true;
		}
		
		
	}
	
	

		
