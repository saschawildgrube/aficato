<?php
	
	require_once(GetWDKDir()."wdk_filesys.inc");
	require_once(GetWDKDir()."wdk_unittest_recursivefilecheck.inc");
	
	class CTest extends CUnitTestRecursiveFileCheck
	{
		function __construct()
		{
			parent::__construct("Check for bad whitespace characters in source code files");
		}
		
		function Callback_TestCase_CheckFile($strFilePath)
		{ 
			$strExtention = GetExtentionFromPath($strFilePath);
			if (		$strExtention == "inc"
					|| 	$strExtention == "php")
			{
				$strSourceCode = FileRead($strFilePath);
				$strSourceCodeTrimmed = StringRemoveLeadingCharacters($strSourceCode,CHARSET_WHITESPACE);
				if ($strSourceCode != $strSourceCodeTrimmed)
				{
					$this->Trace("strFilePath = \"$strFilePath\"");
					$this->Trace("There are whitespaces before the first printable character.");
					$this->Trace("Testcase FAILED!");	
					$this->Trace("");
					$this->Trace("");
					$this->SetResult(false);
					return;
				}
			}
		}

		function CallbackTest()
		{
			parent::CallbackTest();
			$this->SetResult(true);
			$this->CheckSourceDirectories();
		}
	}
	
	

		
