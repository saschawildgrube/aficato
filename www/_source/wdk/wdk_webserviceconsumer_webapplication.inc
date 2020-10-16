<?php

	require_once("wdk_webserviceconsumer.inc");
	
	class CWebServiceConsumerWebApplication extends CWebServiceConsumer
	{
		private $m_webapp;
		private $m_bTrace;
		
		function __construct(CWebApplication $webapp)
		{
			parent::__construct();
			$this->m_bTrace = true;
			$this->m_webapp = $webapp;	
		}
		
		function ConsumeWebService(
			$strWebService,
			$arrayParams,
			$nTimeOut=WEBSERVICE_CONSUMER_TIMEOUT_DEFAULT)
		{
			$this->ProfilingTrace();
			$this->Trace("ConsumeWebService BEGIN");
			$this->m_webapp->ProfilingWebservicesStart();
	
			if (!is_array($arrayParams))
			{
				$arrayParams = array();
			}
			
			if (!ArrayKeyExists($arrayParams,"trace"))
			{
				$arrayParams["trace"] = ($this->m_webapp->IsTraceActive())?("1"):("0");
			}

/*
			if (!ArrayKeyExists($arrayParams,"nocache"))
			{
				$arrayParams["nocache"] = (GetBoolValue($this->m_webapp->GetParam("nocache")))?("1"):("0");
			}
*/
			
			if (IsValidURL($strWebService))
			{
				$strWebServiceURL = $strWebService;	
			}
			else
			{
				$arrayWebServices = $this->m_webapp->GetConfig("webservices");
				if (!is_array($arrayWebServices))
				{
					$this->Trace("GetConfig(\"webservices\") did not return an array!");
					$this->Trace("ConsumeWebService END");
					return false;	
				}
				$strWebServiceURL = ArrayGetValue(
					$arrayWebServices,
					$strWebService,
					"url");
					
				$strWebServiceAccessCode = ArrayGetValue(
					$arrayWebServices,
					$strWebService,
					"accesscode");
				if ($strWebServiceAccessCode != "")
				{
					$arrayParams["accesscode"] = $strWebServiceAccessCode;
				}
				
				if (ArrayGetValue($arrayParams,"format") == "")
				{
					$strWebServiceFormat = ArrayGetValue(
						$arrayWebServices,
						$strWebService,
						"format");
					if ($strWebServiceFormat != "")
					{
						$arrayParams["format"] = $strWebServiceFormat;
					}
				}

				
			}
			
			if (!ArrayKeyExists($arrayParams,"format"))
			{
				//$arrayParams["format"] = "csv";   
			}
			
			$this->Trace("Service URL: \"$strWebServiceURL\"");
			$this->Trace("Parameter:");
			$this->Trace($arrayParams);
			
			$bResult = parent::ConsumeWebService($strWebServiceURL,$arrayParams,$nTimeOut);

			if ($bResult == false)
			{
				$this->Trace("ERROR: ".$this->GetError());
			}

			$this->Trace($this->GetServiceOutput());
			//$this->Trace($this->m_arrayResultEnvelope);
			$this->Trace("Seconds elapsed during request: ".RenderNumber($this->GetSecondsRequest(),3));
			$this->Trace("Seconds elapsed during parsing: ".RenderNumber($this->GetSecondsParsing(),3));
			$this->Trace("Seconds elapsed during script : ".RenderNumber($this->GetSecondsScript(),3));
			$this->m_webapp->ProfilingWebservicesStop();
			$this->Trace("ConsumeWebService END");
			$this->Trace("");
			$this->ProfilingTrace();	   		
			return $bResult;
		}
		
		function GetResultArrayInsecure()
		{
			$this->Trace("WARNING: Use of GetResultArrayInsecure() is a possible security contraint!");
			return parent::GetResultArrayInsecure();
		}
		
		function SetTrace($bActive)
		{
			$this->m_bTrace = $bActive;	
		}
		
		function Trace($value)
		{
			if ($this->m_bTrace == false)	
			{
				return;	
			}
			$this->m_webapp->Trace($value);
		}
		
		function ProfilingTrace()
		{
			if ($this->m_bTrace == false)	
			{
				return;	
			}
			$this->m_webapp->ProfilingTrace();
		}
		
	}



