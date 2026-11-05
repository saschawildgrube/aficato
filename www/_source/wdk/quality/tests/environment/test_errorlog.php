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
				$this->Trace("Log file does not exist or the system user does not have access.");
				$this->SetResult(false);
				return;
			}	
			
			$nLogLength = StringLength($strLog);
			$this->Trace("Log file size: ".RenderValue($nLogLength));
			
			if ($nLogLength > 2)
			{
				$nSectionLength = 10000;
				$this->Trace("Found at least one entry in the error log:");
				$strLogSection = StringSection($strLog,0,$nSectionLength);
				if (StringLength($strLogSection) > $nSectionLength)
				{
					$strLogSection .= "...";
				}
				$this->Trace($strLogSection);
				$this->SetResult(false);	
				return;
			}
		}
	}
		
