<?php
	
	require_once(GetWDKDir()."wdk_filesys.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Check error log");
		}

		function OnTest() 
		{
			parent::OnTest();

			$this->SetResult(true);
			
			$strLogFile = GetErrorLogPath();
			if ($strLogFile == "")
			{
				$this->Trace("No log file specified by GetErrorLogPath(). Test is set to INACTIVE.");	
				$this->SetActive(false);
				return;
			}
			$this->Trace("Log file: ".$strLogFile);

			$strLog = FileRead($strLogFile,true);
			if ($strLog === false)
			{
				$this->Trace("Log file could not be read");
				$this->Trace("Log file does not exist or the www-data user does not have access.");
				$this->SetResult(false);
				return;
			}	
			
			$this->Trace("Log file size: ".RenderValue(StringLength($strLog)));
			$this->Trace("Searching log for \"PHP \" to check if there are any PHP notices, warnings, or errors.");
			$this->Trace("");
 			
			$nPos = FindString($strLog,"PHP ");
			if ($nPos != -1)
			{
				$this->Trace("Found at least one PHP notice, warning, or error:");
				$nPosNextEOL = FindString(StringSection($strLog,$nPos),"\n");
				$this->Trace("");
				if ($nPosNextEOL != -1)
				{
					$nPosLine = max(0,$nPos-58);
					$this->Trace(StringSection($strLog,$nPosLine,$nPos+$nPosNextEOL-$nPosLine));
					$this->Trace("");
				}
				$this->SetResult(false);	
				return;
			}
		}
	}
		
