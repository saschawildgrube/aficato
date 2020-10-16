<?php

	require_once(GetWDKDir()."wdk_webservice.inc");
	
   class CDemoWebService extends CWebService
	{
		function __construct(
			$strSourceDir,
			$strServiceDir,
			$arrayConfig,
			$arrayParams)
		{
			$arrayMeta = array();
			$arrayMeta["version"] = WDK_VERSION; 
			$arrayMeta["author"] = "Sascha Wildgrube";
			
			parent::__construct(
				"demo/demo",
				$strSourceDir,
				$strServiceDir,
				$arrayMeta,
				$arrayConfig,
				$arrayParams,
				array()
				);
		}
		
		function CallbackInitParameterDefinition(&$paramDef)
		{
			parent::CallbackInitParameterDefinition($paramDef);
		}
		
		function CallbackInitCommandDefinition(&$commandDef)
		{
			$commandDef->AddCommandDefinition("demo",array());
			parent::CallbackInitCommandDefinition($commandDef);
		}
		
		
		function CallbackInit()
		{
			return parent::CallbackInit();
		}
		
		
		function CallbackCommand($strCommand)
		{
			switch ($strCommand)
			{
			case "demo":
				$this->CommandDemo();
				return;

			default:
				parent::CallbackCommand($strCommand);
				return;
			}
		}
		
		function CommandDemo()
		{


			$this->Trace("Lets add some bogus output!");
			
			$arrayList = array();
			
			for ($nIndex = 0; $nIndex < 3; $nIndex++)
			{
				$element = array();
				$element["TAG"] = "Tag$nIndex";
				$element["VALUE"] = "Value$nIndex"; 
				

				array_push($arrayList,$element);
			}
			

			
			$this->SetResultValue($arrayList,"LIST");
			
			
			$this->SetResultValue("Test","TEST1");
			$this->SetResultValue("Text is encapsulated like this \"<![CDATA[This is a test]]>\".","TEST2");
			$this->SetResultValue(utf8_encode("ÄÜÖäüö"),"TEST3");
			$this->SetResultValue("<>/&\"\'","TEST4");
			$this->SetResultValue(utf8_encode("ä"),"TEST5");
			$this->SetResultValue("uppercase?","test6");

			
		}
	
	
		
	} 

