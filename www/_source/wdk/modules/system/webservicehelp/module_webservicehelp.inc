<?php

	class CWebserviceHelpModule extends CModule
	{
		private $m_strURL;
		
		function __construct(
			CWebSite $website,
			$strURL)
		{
			$this->m_strURL = $strURL;
			parent::__construct($website,"system/webservicehelp");
		}
		
		function CallbackInit()
		{
			return true;
		}

		function CallbackRenderModule($strState)
		{
			$this->SetOutputItem("URL",$this->m_strURL);
			
			$consumer = new CWebServiceConsumerWebApplication($this->GetWebsite());	
			 
			$arrayParams = array();
			$arrayParams["command"] = "help";
		
			$consumer->ConsumeWebService($this->m_strURL,$arrayParams);
			$strError = $consumer->GetError();
						
			if ($strError != "")
			{
				$this->SetErrorMessageCode("UNKNOWN");
				$this->SetOutputItem("HELP","");
				return "default";
			}
			
			$this->SetOutputItem("HELP",HtmlEncode($consumer->GetHelp()));
			return "default";
		}
	}



