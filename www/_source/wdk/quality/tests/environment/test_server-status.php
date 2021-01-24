<?php
	
	require_once(GetWDKDir()."wdk_unittest_monitoring.inc");
	require_once(GetSourceDir()."webservices_directory.inc");
		
	class CTest extends CMonitoringUnitTest
	{
		function __construct()
		{
			$arrayConfig = array();
			$arrayConfig["webservices"] = GetWebServicesDirectory();			
			parent::__construct("Check Server Status",$arrayConfig);
		}
		

		function OnTest()
		{
			parent::OnTest();
			$this->SetResult(true);
			$strURL = $this->GetWebserviceURL("system/server");
			if ($strURL == "")
			{
				$this->SetActive(false);
				return;
			}
			$this->TestCase_Server($strURL,$this->GetWebserviceAccessCode("system/server"),false);
		}
		
			
	}
	
	

		
