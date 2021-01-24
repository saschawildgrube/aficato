<?php
	
	require_once(GetSourceDir()."webservices_directory.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			$arrayConfig = array();
			$arrayConfig["webservices"] = GetWebServicesDirectory();			
			parent::__construct("Check Server Performance",$arrayConfig);
		}
		

		function OnTest()
		{
			parent::OnTest();

			$this->SetResult(true);					

			$webservice = new CWebServiceConsumerWebApplication($this);

			$arrayParams = array();
			$arrayParams["command"] = "performance";
			$webservice->ConsumeWebService("system/server",$arrayParams);
			if ($webservice->GetError() != "")	
			{
				$this->Trace("Error: \"".$webservice->GetError()."\"");
				$this->SetResult(false);					
				return;	
			}

			$this->Trace("Checking Counting Speed");
			$fCountingSpeedSeconds = $webservice->GetResultValue("COUNTING_SPEED_SECONDS");
			if ($fCountingSpeedSeconds > 1)
			{
				$this->Trace("Server took more than a seconds to count to 1 MM. That is SLOW!"); 
				$this->SetResult(false);
			}
		}
		
			
	}
	
	

		
