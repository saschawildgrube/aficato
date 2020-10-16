<?php

	abstract class CWebsiteSatellite implements IEventHandler
	{
		private $m_website;
		private $m_strID;
		private $m_strResPrefix;
		
		protected $m_arrayOutputItems;
		
		function __construct(
			CWebSite &$website,
			$strID)
		{
			if (!is_subclass_of($website,'CWebsite'))
			{
				FatalError('CWebsiteSatellite "'.$strID.'" has been initialized with an invalid website object.');
			}
			$this->m_website = $website;
			$this->m_strID = $strID;
			$this->m_strResPrefix = '';
			$this->m_arrayOutputItems = array();
		}
		
		final
		function GetWebsite()
		{
			return $this->m_website;
		}
		
		final
		function GetAssemblyLine()
		{
			return $this->GetWebsite()->GetAssemblyLine(); 
		}

		final
		function GetID()
		{
			return $this->m_strID;	
		}
		
		function GetDataFilePath($strFilePath)
		{
			return $this->GetAssemblyLine()->GetDataFilePath($strFilePath);	
		}
		
		
		function Error($strError)
		{
			$this->GetWebsite()->Error($this->GetID().": ".$strError);	
		}
		
		function SetResPrefix($strResPrefix)
		{
			$this->m_strResPrefix = $strResPrefix;
		}

		function AddErrorMessageCode($strErrorMessageCode)
		{
			$this->GetWebsite()->AddErrorMessageCode($strErrorMessageCode,StringUpperCase($this->m_strResPrefix));
		}
		function AddWarningMessageCode($strWarningMessageCode)
		{
			$this->GetWebsite()->AddWarningMessageCode($strWarningMessageCode,StringUpperCase($this->m_strResPrefix));
		}
		function AddInfoMessageCode($strInfoMessageCode)
		{
			$this->GetWebsite()->AddInfoMessageCode($strInfoMessageCode,StringUpperCase($this->m_strResPrefix));
		}

		function AddErrorMessageText($strErrorMessageText)
		{
			$this->GetWebsite()->AddErrorMessageText($strErrorMessageText);
		}
		function AddWarningMessageText($strWarningMessageText)
		{
			$this->GetWebsite()->AddWarningMessageText($strWarningMessageText);
		}
		function AddInfoMessageText($strInfoMessageText)
		{
			$this->GetWebsite()->AddInfoMessageText($strInfoMessageText);
		}



		function SetErrorMessageCode($strErrorMessageCode)
		{
			$this->GetWebsite()->SetErrorMessageCode($strErrorMessageCode,StringUpperCase($this->m_strResPrefix));
		}
		function SetWarningMessageCode($strWarningMessageCode)
		{
			$this->GetWebsite()->SetWarningMessageCode($strWarningMessageCode,StringUpperCase($this->m_strResPrefix));
		}
		function SetInfoMessageCode($strInfoMessageCode)
		{
			$this->GetWebsite()->SetInfoMessageCode($strInfoMessageCode,StringUpperCase($this->m_strResPrefix));
		}

		function SetErrorMessageText($strErrorMessageText)
		{
			$this->GetWebsite()->SetErrorMessageText($strErrorMessageText);
		}
		function SetWarningMessageText($strWarningMessageText)
		{
			$this->GetWebsite()->SetWarningMessageText($strWarningMessageText);
		}
		function SetInfoMessageText($strInfoMessageText)
		{
			$this->GetWebsite()->SetInfoMessageText($strInfoMessageText);
		}

		function GetOutputItem($strItem)
		{
			return ArrayGetValue($this->m_arrayOutputItems,$strItem);	
		}

		function SetOutputItem($strItem, $strContents)
		{
			$this->m_arrayOutputItems[$strItem] = $strContents;	
		}
		function SetOutputItems($arrayItems)
		{
			$this->m_arrayOutputItems = ArrayMerge($this->m_arrayOutputItems,$arrayItems);	
		}
		function ReplaceOutputItems($strSource)
		{
		   return ReplaceTagsWDK($strSource,$this->m_arrayOutputItems);
		}		

		function IsParam($strParam)
		{
			return $this->GetWebsite()->IsParam($strParam);
		}

		function GetParam($strParam)
		{
			return $this->GetWebsite()->GetParam($strParam);
		}
		function GetConfig($strConfig1,$strConfig2="",$strConfig3="",$strConfig4="")
		{
			return $this->GetWebsite()->GetConfig($strConfig1,$strConfig2,$strConfig3,$strConfig4);
		}		
		function GetWebserviceURL($strWebservice)
		{
			return $this->GetWebsite()->GetWebserviceURL($strWebservice);
		}

		function IsVar($strVar)
		{
			return $this->GetWebsite()->IsVar($strVar);
		}
		function GetVar($strVar)
		{
			return $this->GetWebsite()->GetVar($strVar);
		}
		function SetVar($strVar,$strValue)
		{
			$this->GetWebsite()->SetVar($strVar,$strValue);
		}
		
		
		function LoadStringResources($strID,$strLanguage = '')
		{
			return $this->GetWebsite()->LoadStringResources($strID,$strLanguage);
		}
		
		function LoadString($strID,$strLanguage = '')
		{
			return $this->GetWebsite()->LoadString($strID,$strLanguage);
		}
		
		function Trace($value)
		{
			$this->GetWebsite()->Trace($this->GetID().': '.RenderValue($value));
		}
		function ProfilingTrace()
		{
			$this->GetWebsite()->ProfilingTrace();	
		}
		function Debug($value = null)
		{
			$this->GetWebsite()->Debug($value);	
		}
		
		
		function EventHandler($strEventID,$arrayParams)
		{
		}
		
		function IncludeElement($strElementID)
		{
			return $this->GetWebsite()->IncludeElement($strElementID);
		}
		function IncludeModule($strModuleID)
		{
			return $this->GetWebsite()->IncludeModule($strModuleID);
		}
		
		function SystemLog($strEventSeverity,$strEventID,$strEventMessage,$arrayDetails=array())
		{
			return $this->GetWebsite()->SystemLog($strEventSeverity,$strEventID,$strEventMessage,$arrayDetails);
		}
		function SystemEvent($strEventSeverity,$strEventID,$strEventMessage,$arrayDetails=array())
		{
			return $this->GetWebsite()->SystemEvent($strEventSeverity,$strEventID,$strEventMessage,$arrayDetails);
		}
		
		function MakeURL($arrayParams,$bEncoded=true)
		{
			return $this->GetWebsite()->MakeURL($arrayParams, $bEncoded);
		}
		
		
		function AddJavaScript($strTitle,$strID='')
		{
			$this->GetWebsite()->AddJavaScript($strTitle,$strID); 
		}

		function AddJavaScriptURL($strTitle,$strURL)
		{
			$this->GetWebsite()->AddJavaScriptURL($strTitle,$strURL);		
		}

		function AddJavaScriptFile($strTitle,$strFilePath)
		{
			$this->GetWebsite()->AddJavaScriptFile($strTitle,$strFilePath);
		}

		function AddJavaScriptSourceCode($strTitle,$strSourceCode)
		{
			$this->GetWebsite()->AddJavaScriptSourceCode($strTitle,$strSourceCode);
		}
		
		function AddStyleSheet($strTitle,$strID='')
		{
			$this->GetWebsite()->AddStyleSheet($strTitle,$strID);
		}

		function AddStyleSheetFilePath($strTitle,$strFilePath)
		{
			$this->GetWebsite()->AddStyleSheetFilePath($strTitle,$strFilePath);
		}

		function AddStyleSheetURL($strTitle,$strURL)
		{
			$this->GetWebsite()->AddStyleSheetURL($strTitle,$strURL);
		}

		function AddStyleSheetSourceCode($strTitle,$strSourceCode)
		{
			$this->GetWebsite()->AddStyleSheetSourceCode($strTitle,$strSourceCode);
		}

		function RenderDate($time) 
		{
			return $this->GetWebsite()->RenderDate($time);
		}

		function RenderTime($time)
		{
			return $this->GetWebsite()->RenderTime($time);
		}
		
		function RenderDateTime($time)
		{
			return $this->GetWebsite()->RenderDateTime($time);
		}
		
		function RenderNumber($fValue,$nPrecision=0,$strMetric='',$bUseSeparator = true)
		{
			return $this->GetWebsite()->RenderNumber($fValue,$nPrecision,$strMetric,$bUseSeparator);
		}

		function RenderCurrency($fValue,$strCurrencyID)
		{
			return $this->GetWebsite()->RenderCurrency($fValue,$strCurrencyID);
		}
		
	}



