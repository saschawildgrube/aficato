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
			$nTimeOut = WEBSERVICE_CONSUMER_TIMEOUT_DEFAULT,
			$strTransportMode = null)
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
			
			$arrayWebServices = $this->m_webapp->GetConfig("webservices");
			$strConfiguredService = $strWebService;
			if (IsValidURL($strWebService))
			{
				$strConfiguredService = '';
				if (is_array($arrayWebServices))
				{
					foreach ($arrayWebServices as $strName => $arrayService)
					{
						if (ArrayGetValue($arrayService,'url') == $strWebService)
						{
							$strConfiguredService = $strName;
							break;
						}
					}
				}
			}
			if ($strTransportMode === null)
			{
				$strTransportMode = ArrayGetValue($arrayWebServices,$strConfiguredService,'transport');
				if ($strTransportMode == '')
				{
					$strTransportMode = $strConfiguredService != '' && function_exists('GetWebservicesDefaultTransportMode')
						? GetWebservicesDefaultTransportMode() : 'http';
				}
			}

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
					
				
				$strWebServiceAccessCode = $this->m_webapp->GetWebserviceAccessCode($strWebService);
					/*
				$strWebServiceAccessCode = ArrayGetValue(
					$arrayWebServices,
					$strWebService,
					"accesscode");
					*/
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
			$this->Trace("Transport: ".$strTransportMode);
			$this->Trace("Parameter:");
			$this->Trace($arrayParams);
			
			$bResult = parent::ConsumeWebService($strWebServiceURL,$arrayParams,$nTimeOut,$strTransportMode);

			if ($bResult == false)
			{
				$this->Trace("ERROR: ".$this->GetError());
				if ($this->GetTransportError() != "")
				{
					$this->Trace("Transport failure: ".$this->GetTransportError());
				}
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



