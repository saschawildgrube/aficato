<?php
	
	//require_once(GetSourceDir()."webservices_directory.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			$arrayConfig = array();
			//$arrayConfig["webservices"] = GetWebServicesDirectory();
			parent::__construct("Test Web services",$arrayConfig);
		}
	
		function CallbackInit()
		{
			$this->SetVerbose(true);
			$this->SetResult(true);
			return parent::CallbackInit();	
		}
	
		function CallbackTest()
		{
			parent::CallbackTest();	
	
			//$arrayWebservices = $this->GetConfig("webservices");
			//$this->Trace($arrayWebservices);
				

			$this->Trace("Executing ConsumeWebService without an URL.");
			$arrayParams = array();
			$consumer = new CWebServiceConsumer($this);
			$bResult = $consumer->ConsumeWebService("",$arrayParams);
			if ($bResult != false)
			{
				$this->Trace("Unexpected result!");		
				$this->SetResult(false);
				return;	
			}
			if ($consumer->GetError() != "URL_MISSING")	
			{
				$this->Trace("Unexpected error code: \"".$consumer->GetError()."\"");		
				$this->SetResult(false);
				return;	
			}

		}
		
		
	}
	
		
	
	
