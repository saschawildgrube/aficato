<?php
	
	require_once(GetWDKDir()."wdk_system.inc");
		
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("GetSystemUser");
		}
		
		function OnInit()
		{
			parent::OnInit();

			if (CheckSafeMode())
			{
				$this->Trace("Safe Mode is ON. So Shell* functions are not expected to work!");
				$this->SetActive(false);
			}
			
			return true;
		}

		function OnTest()
		{
			parent::OnTest();
			$this->SetResult(true);
			$strUser = GetSystemUser();
			$this->Trace("GetSystemUser() returned \"$strUser\"");
			if ($strUser == "")
			{
				$this->Trace("GetSystemUser() failed!");	
				$this->SetResult(false);
			}
		}
	}
	
	

		
