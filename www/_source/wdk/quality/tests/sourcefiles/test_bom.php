<?php
	
	require_once(GetWDKDir()."wdk_utf8.inc");
	require_once(GetWDKDir()."wdk_unittest_recursivefilecheck.inc");
	
	class CTest extends CUnitTestRecursiveFileCheck
	{
		function __construct()
		{
			parent::__construct("Check for BOM (Byte Order Mask) in source, html, and text files");
		}
		
		function Callback_TestCase_CheckFile($strFilePath)
		{ 
			$strExtention = GetExtentionFromPath($strFilePath);
			if (	$strExtention == "inc"
				||  $strExtention == "php"
				||  $strExtention == "htm"
				||  $strExtention == "txt")
			{
				if (FindString($strFilePath,"bom.txt") != -1)
				{
					$this->Trace("strFilePath = \"$strFilePath\"");
					$this->Trace("File is ignored");
					return;
				}
				if (IsFileUTF8BOM($strFilePath))
				{
					$this->Trace("strFilePath = \"$strFilePath\"");
					$this->Trace("A BOM (Byte Order Mask) has been found!");
					$this->Trace("Testcase FAILED!");	
					$this->Trace("");
					$this->Trace("");
					$this->SetResult(false);
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
	
	

		
