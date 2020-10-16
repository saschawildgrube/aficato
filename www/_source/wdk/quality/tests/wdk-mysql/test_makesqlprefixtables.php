<?php
	
	require_once(GetWDKDir()."wdk_mysql.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("MakeSQL_PrefixTables");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_MakeSQL_PrefixTables($strSQL,$strTablePrefix,$strExpectedResult)
		{
			$this->Trace("TestCase_MakeSQL_PrefixTables");
			$this->Trace("strTablePrefix: \"$strTablePrefix\"\n");
			$this->Trace("strSQL:\n$strSQL\n");
			$this->Trace("Expected Result:\n$strExpectedResult");
			$strResult = MakeSQL_PrefixTables($strSQL,$strTablePrefix);
			$this->Trace("MakeSQL_PrefixTables() returns:\n$strResult\n");
			if ($strResult == $strExpectedResult)
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

			$this->TestCase_MakeSQL_PrefixTables(
				"",
				"",
				"");
		
			$this->TestCase_MakeSQL_PrefixTables(
				"SELECT * FROM /*TablePrefix*/`Table`;",
				"Prefix-",
				"SELECT * FROM `Prefix-Table`;");

			$this->TestCase_MakeSQL_PrefixTables(
				"INSERT INTO /*TablePrefix*/`System-Session` SET `SESSION_ID` = 'f1123778e168013b380d6d6d2a273ba8c0f39905', `OWNER_ID` = 'WEBSITE_WDK', `CREATION_DATETIME` = '2013-02-12 00:24:49', `LASTACCESS_DATETIME` = '2013-02-12 00:24:49', `END_DATETIME` = '2013-02-12 12:24:49', `DATA` = '';",
				"Prefix-",
				"INSERT INTO `Prefix-System-Session` SET `SESSION_ID` = 'f1123778e168013b380d6d6d2a273ba8c0f39905', `OWNER_ID` = 'WEBSITE_WDK', `CREATION_DATETIME` = '2013-02-12 00:24:49', `LASTACCESS_DATETIME` = '2013-02-12 00:24:49', `END_DATETIME` = '2013-02-12 12:24:49', `DATA` = '';");

		}
		

	}
	
	

		
