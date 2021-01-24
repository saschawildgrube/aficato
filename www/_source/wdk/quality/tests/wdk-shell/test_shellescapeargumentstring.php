<?php
	
	require_once(GetWDKDir()."wdk_shell.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("ShellEscapeArgumentString");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_ShellEscapeArgumentString($strArgument,$strExpectedResult)
		{
			$this->Trace('TestCase_ShellEscapeArgumentString');
			$this->Trace("strArgument:                  \"$strArgument\"");
			$this->Trace("Expected Result:              \"$strExpectedResult\"");
			$strResult = ShellEscapeArgumentString($strArgument);
			$this->Trace("ShellEscapeArgumentString():  \"$strResult\"");
			$this->Trace("escapeshellarg():             \"".escapeshellarg($strArgument)."\"");
			if ($strResult == StringEncodeUTF8($strExpectedResult))
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

	
		function OnTest()
		{
			parent::OnTest();
			
		
			$this->TestCase_ShellEscapeArgumentString(
				"abc",
				"'abc'");

			$this->TestCase_ShellEscapeArgumentString(
				"abc ",
				"'abc '");

			$this->TestCase_ShellEscapeArgumentString(
				" abc ",
				"' abc '");

			$this->TestCase_ShellEscapeArgumentString(
				" הצü ",
				"' הצü '");

			$this->TestCase_ShellEscapeArgumentString(
				"' הצü '",
				"''\'' הצü '\'''");

			$this->TestCase_ShellEscapeArgumentString(
				u("' הצü '"),
				u("''\'' הצü '\'''"));

			$this->TestCase_ShellEscapeArgumentString(
				"' הצü \n\t\"'",
				"''\'' הצü \n\t\"'\'''");



		}
		

	}
	
	

		
