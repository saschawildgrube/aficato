<?php
	
	require_once(GetWDKDir()."wdk_unittest_recursivefilecheck.inc");
	
	class CTest extends CUnitTestRecursiveFileCheck
	{
		function __construct()
		{
			parent::__construct("Check for unintended folders");
		}
		
		function OnTestCaseCheckFolder($strFolderPath)
		{
			$bUnintendedFolder = false;
			$strFolderName = GetFolderNameFromPath($strFolderPath);
			
			
			// Google Drive sometimes creates folder copies by error
			if (RegExpMatch($strFolderName,'/\([0-9]+\)/m') == true)
			{
				$bUnintendedFolder = true;
			}

			
			if ($bUnintendedFolder == true)
			{
				$this->Trace("$strFolderPath");
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
	
	

		
