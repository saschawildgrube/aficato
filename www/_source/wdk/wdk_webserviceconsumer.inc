<?php

	require_once("wdk_json.inc");
	require_once("wdk_xml.inc");
	require_once("wdk_pathcontent.inc");
	require_once("wdk_csv.inc");
	
	define("WEBSERVICE_CONSUMER_TIMEOUT_DEFAULT",180);
	
	function IsValidWebserviceName($strWebserviceName)
	{
		if ($strWebserviceName == "")	
		{
			return false;
		}
		if (StringCheckCharSet($strWebserviceName,CHARSET_WEBSERVICENAME) == false)
		{
			return false;	
		}
		if (
					(StringSection($strWebserviceName,0,1) == "/")
			|| 	(StringSection($strWebserviceName,-1,1) == "/")
			|| 	(FindString($strWebserviceName,"/") == -1) 
			)
		{
			return false;
		}
		return true;
	}
	
	class CWebServiceConsumer
	{
		private $m_arrayParams;
		private $m_strWebServiceURL;
		
		private $m_bCalled;
		private $m_strError;
		
		private $m_arrayResultEnvelope;
		private $m_strServiceOutput;
		
		protected $m_stopwatchRequest;
		protected $m_stopwatchParsing;
		protected $m_fSecondsScript;

		
		function __construct()
		{
			$this->m_bCalled = false;	
			$this->m_strError = "";
			$this->m_stopwatchRequest = new CStopWatch();
			$this->m_stopwatchParsing = new CStopWatch();
			$this->m_fSecondsScript = 0;
			
		}
		
		function ConsumeWebService($strWebServiceURL,$arrayParams,$nTimeOut=WEBSERVICE_CONSUMER_TIMEOUT_DEFAULT)
		{
			$this->m_bCalled = true;
			
			$this->m_arrayParams = $arrayParams;
			$this->m_strWebServiceURL = $strWebServiceURL;
			
			$this->m_strServiceOutput = "";
			$this->m_arrayResultEnvelope = array();
			
			$this->m_strError = "";
			
			$this->m_fSecondsScript = 0;
			
			$nTimeOut = intval($nTimeOut);
			if ($nTimeOut < 0)
			{
				$nTimeOut = WEBSERVICE_CONSUMER_TIMEOUT_DEFAULT;
			}
			if ($nTimeOut > 600) 
			{
				$nTimeOut = 600;
			}
			

		
			$strURL = $strWebServiceURL;
			
			if ($strURL == "")
			{
				$this->m_strError = "URL_MISSING";
				return false;	
			}

			$this->m_stopwatchRequest->Start();
			$strServiceOutput = HttpRequest($strURL,$arrayParams,"post",$nTimeOut);
			$this->m_stopwatchRequest->Stop();

			if ($strServiceOutput == false)
			{
				$this->m_strError = "REQUEST_FAILED";
				return false;
			}
			
			
			$this->m_strServiceOutput = $strServiceOutput;
			
			$strFormat = "";

			if (StringSection($this->m_strServiceOutput,0,2) == "{\"")
			{
				$strFormat = "json";
			}
			else if (StringSection($this->m_strServiceOutput,0,7) == "{\n    \"")
			{
				$strFormat = "json";
			}
			else if (StringSection($this->m_strServiceOutput,0,5) == "<?xml")
			{
				$strFormat = "xml";
			}
			else if (StringSection($this->m_strServiceOutput,0,9) == "META/NAME")
			{
				$strFormat = "csvpath";	
			}
			else if (StringSection($this->m_strServiceOutput,0,1) == "\"")
			{
				$strFormat = "csv";	
			}
			else
			{
				$this->m_strError = "FORMAT_INVALID";
				return false;	
			}

			$this->m_stopwatchParsing->Start();
			
			switch ($strFormat)
			{
			case "xml":
				$xml = new CXMLElement();
				$arrayData = array();
				$arrayIndex = array();
				if ($xml->Parse($strServiceOutput) == false)
				{
					$this->m_strError = "XML_PARSING";
					$this->m_stopwatchParsing->Stop();
					return false;
				}
				$this->m_arrayResultEnvelope = $xml->GetRecursiveArray();
				break;

			case "csvpath":
				$this->m_arrayResultEnvelope = ParsePathContent($this->m_strServiceOutput);
				if (!is_array($this->m_arrayResultEnvelope) || ArrayCount($this->m_arrayResultEnvelope) == 0)
				{
					$this->m_arrayResultEnvelope = array();
					$this->m_strError = "CSVPATH_PARSING";
					$this->m_stopwatchParsing->Stop();
					return false;
				}
				if (!IsValidWebserviceName(ArrayGetValue($this->m_arrayResultEnvelope,"META","NAME")))
				{
					$this->m_arrayResultEnvelope = array();
					$this->m_strError = "CSVPATH_INVALIDNAME";
					$this->m_stopwatchParsing->Stop();
					return false;
				}
				$this->m_fSecondsScript = floatval(ArrayGetValue($this->m_arrayResultEnvelope,"TOTAL_ELAPSED_SECONDS"));				
				break;

			case "json":
				$this->m_arrayResultEnvelope = JsonDecode($this->m_strServiceOutput);
				if (!is_array($this->m_arrayResultEnvelope))
				{
					$this->m_arrayResultEnvelope = array();
					$this->m_strError = "JSON_DECODE";
					$this->m_stopwatchParsing->Stop();
					return false;
				}
				break;
			
			case "csv":
				$arrayResult = ParseCSV_RFC4180($this->m_strServiceOutput);
				if ($arrayResult == false)
				{
					$this->m_arrayResultEnvelope = array();
					$this->m_strError = "CSV_PARSING";
					$this->m_stopwatchParsing->Stop();
					return false;
				}
				if (ArrayCount($arrayResult) == 1)
				{
					$arrayRow = ArrayGetValue($arrayResult,0);
					if (!is_array($arrayRow))
					{
						$this->m_arrayResultEnvelope = array();
						$this->m_strError = "CSV_PARSING";
						$this->m_stopwatchParsing->Stop();
						return false;
					}
					if (ArrayCount($arrayRow) == 1)
					{
						$strError = ArrayGetValue($arrayRow,"ERROR");
						if ($strError != "")	
						{
							$this->m_arrayResultEnvelope["ERROR"] = $strError;
						}
					}
				}
				if (ArrayGetValue($this->m_arrayResultEnvelope,"ERROR") == "")
				{
					// Although this is assumptive, this is probably the best fitting default:
					$this->m_arrayResultEnvelope["RESULT"]["LIST"] = $arrayResult;
					
					// Here again, one row MAY be the indicator for a non-list like result, so let's try to immitate tree structure behaviour:
					if (ArrayCount($arrayResult) == 1)
					{
						$arrayRow = ArrayGetValue($arrayResult,0);
						if (is_array($arrayRow))
						{
							$this->m_arrayResultEnvelope["RESULT"] = ArrayAppend($this->m_arrayResultEnvelope["RESULT"],$arrayRow);	
						}
					}
				}
				break;
				
			default:
				break;
			}
			
			$this->m_stopwatchParsing->Stop();
			
			$this->m_strError = ArrayGetValue($this->m_arrayResultEnvelope,"ERROR");		
			if ($this->m_strError != "")
			{
				return false;	
			}
			
			$strMetaName = ArrayGetValue($this->m_arrayResultEnvelope,"META","NAME");
			if ($strMetaName == "" && $strFormat != "csv")
			{
				$this->m_strError = "RESULT_INVALID";
				return false;
			}
			return true;
		}
		
		function GetError()
		{
			if ($this->m_bCalled == false)
			{
				return "";
			}
			return $this->m_strError;
		}

		function GetTrace()
		{
			if ($this->m_bCalled == false)
			{
				return "";
			}
			return ArrayGetValue($this->m_arrayResultEnvelope,"TRACE");
		}

		function GetHelp()
		{
			if ($this->m_bCalled == false)
			{
				return "";
			}
			return ArrayGetValue($this->m_arrayResultEnvelope,"HELP");
		}

		function GetResultArray()
		{
			return SecureOutput(ArrayGetValue($this->m_arrayResultEnvelope,"RESULT"));
		}
		function GetResultArrayInsecure()
		{
			return ArrayGetValue($this->m_arrayResultEnvelope,"RESULT");
		}

		
		function GetResultValue($key1,$key2="",$key3="",$key4="",$key5="",$key6="")
		{
			return SecureOutput(
				ArrayGetValue(
					$this->m_arrayResultEnvelope,
					"RESULT",$key1,$key2,$key3,$key4,$key5,$key6));	
		}
		function GetResultValueInsecure($key1,$key2="",$key3="",$key4="",$key5="",$key6="")
		{
			return ArrayGetValue(
				$this->m_arrayResultEnvelope,
				"RESULT",$key1,$key2,$key3,$key4,$key5,$key6);	
		}		
		
		
		function GetResultList()
		{
			$arrayList = $this->GetResultValue("LIST");
			if (!is_array($arrayList))
			{
				$arrayList = array();
			}
			return $arrayList;
		}
		
		function GetResultListInsecure()
		{
			$arrayList = $this->GetResultValueInsecure("LIST");
			if (!is_array($arrayList))
			{
				$arrayList = array();
			}
			return $arrayList;
		}
		
		


		function GetMetaValue($strElementName)
		{
			if ($this->m_bCalled == false)
			{
				return "";
			}
			return ArrayGetValue($this->m_arrayResultEnvelope,"META",$strElementName);
		}


		function GetServiceOutput()
		{
			if ($this->m_bCalled == false)
			{
				return "";
			}
			return $this->m_strServiceOutput;
		}
		
		function GetSecondsParsing()
		{
			return $this->m_stopwatchParsing->GetSeconds();
		}

		function GetSecondsRequest()
		{
			return $this->m_stopwatchRequest->GetSeconds();
		}

		function GetSecondsScript()
		{
			return $this->m_fSecondsScript;
		}
		
			
		
	}



