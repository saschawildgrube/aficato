<?php
	
	require_once(GetWDKDir()."wdk_unittest_recursivefilecheck.inc");
	
	class CTest extends CUnitTestRecursiveFileCheck
	{
		function __construct()
		{
			parent::__construct("Check for unintended files");
		}
		
		function OnTestCaseCheckFile($strFilePath)
		{
			$bUnintendedFile = false;
			$strExtention = GetExtentionFromPath($strFilePath);
			$strFileName = GetFileNameFromPath($strFilePath);
			
			// No temp files wanted
			if (	$strExtention == "tmp"
				||  $strExtention == "db")
			{
				$bUnintendedFile = true;
			}
			
			// No files allowed that do not match the general file name conventions
			else if (StringCheckCharSet($strFilePath,CHARSET_FILEPATH) == false)
			{
				$bUnintendedFile = true;
			}
			
			// Dropbox sometimes leaves copies of files after unsuccessful merging
			else if (FindString($strFileName,"conflicted copy") != -1)
			{
				$bUnintendedFile = true;
			}

			// Google Drive sometimes leaves copies of files after unsuccessful syncing
			else if (FindString($strFileName,"[Conflict]") != -1)
			{
				$bUnintendedFile = true;
			}

			
			if ($bUnintendedFile == true)
			{
				$this->Trace("$strFilePath");
				$this->SetResult(false);				
			}
		}

		function OnTest()
		{
			parent::OnTest();
			$this->SetResult(true);
			$this->CheckDocumentRootDirectory();  
		}
	}
	
	

		
