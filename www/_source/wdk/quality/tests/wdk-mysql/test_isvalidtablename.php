<?php

	require_once(GetWDKDir()."wdk_mysql.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test SQL_IsValidTableName");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);
			return true;
		}

		function TestCase_SQL_IsValidTableName(
			$value,
			$bExpectedValue)
		{ 
			$this->Trace("TestCase_SQL_IsValidTableName");
		
			$bValue = SQL_IsValidTableName($value);
		
			$this->Trace("SQL_IsValidTableName(".RenderValue($value).") = ".RenderBool($bValue));
		
			if ($bValue == $bExpectedValue)
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
			
			$this->TestCase_SQL_IsValidTableName("System-User",true);
			$this->TestCase_SQL_IsValidTableName("SYSTEM_USER",true);
			$this->TestCase_SQL_IsValidTableName("Table2",true);
			$this->TestCase_SQL_IsValidTableName("T2",true);
			$this->TestCase_SQL_IsValidTableName("t2",true);
			
			
			$this->TestCase_SQL_IsValidTableName("",false);
			$this->TestCase_SQL_IsValidTableName("System.User",false);
			$this->TestCase_SQL_IsValidTableName("abc4567890abc4567890abc4567890abc4567890abc4567890abc456789012345",false);
			$this->TestCase_SQL_IsValidTableName("add",false);
			$this->TestCase_SQL_IsValidTableName("int2",false);
			$this->TestCase_SQL_IsValidTableName("database",false);
			$this->TestCase_SQL_IsValidTableName("DATABASE",false);
			$this->TestCase_SQL_IsValidTableName("Databases",false);
			$this->TestCase_SQL_IsValidTableName("_User",false);
			$this->TestCase_SQL_IsValidTableName("-User",false);
			$this->TestCase_SQL_IsValidTableName("2User",false);
			$this->TestCase_SQL_IsValidTableName(" System-User",false);
			$this->TestCase_SQL_IsValidTableName("System-User ",false);
			
		}
		
		
	}
	
	

		
