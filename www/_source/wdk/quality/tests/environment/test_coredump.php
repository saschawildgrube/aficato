<?php
	
	require_once(GetWDKDir()."wdk_filesys.inc");
	require_once(GetWDKDir()."wdk_system.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Check for core dumps");
		}

		function CallbackTest()
		{
			parent::CallbackTest();

			$this->SetResult(true);
			
			$arrayCoreDumpFiles = GetCoreDumpFiles();
			
			if ($arrayCoreDumpFiles === false)
			{
				$this->Trace("Core dump files could not be checked.");
				$this->Trace("/etc/sudoers.d/wdk must contain the following line:");
				$this->Trace("www-data ALL=(ALL) NOPASSWD: /bin/ls -1 /core.*");
				$this->SetActive(false);
				return;
			}
			else if (ArrayCount($arrayCoreDumpFiles) != 0)
			{
					$this->Trace("There are core dump files on this server! Investigate and clean up!");
					$this->SetResult(false);	
			}
		}
	}
		
