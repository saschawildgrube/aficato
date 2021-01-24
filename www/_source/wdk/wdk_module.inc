<?php

	abstract class CModule extends CWebsiteSatellite
	{
		private $m_strModulePath;
		private $m_strModuleGroup;
		private $m_strModuleID;
		private $m_strModuleDir;
		private $m_bOnInit;
		private $m_bInit;
		private $m_strInitialState;
		private $m_strFinalState;
		private $m_strStyleID;
		private $m_strInstanceID;
		private $m_arrayConfig;
		private $m_arrayToolbarConfig;
		
		function __construct(
			CWebSite &$website,
			$strModulePath,
			$strInstanceID = '',
			$arrayConfig = array())
		{
			
			parent::__construct($website,$strModulePath);
						
			// Init values
			$this->m_strModulePath = $strModulePath;
			$arrayModulePathComponents = explode('/',$strModulePath);
			if (!is_array($arrayModulePathComponents) || ArrayCount($arrayModulePathComponents) != 2)
			{
				$this->Trace($arrayModulePathComponents);
				$this->Error("CModule: Invalid module path: \"".$strModulePath."\"");
			}
			$strModuleGroup = $arrayModulePathComponents[0];
			$strModuleID = $arrayModulePathComponents[1];
			$this->SetResPrefix($strModuleID);
			
			
			$this->m_strModuleID = $strModuleID;
			
			$this->m_strModuleDir = $this->GetWebsite()->GetAssemblyLine()->GetModuleDir($this->m_strModulePath);
			$this->m_strInstanceID = $strInstanceID;
			if (!is_array($arrayConfig))
			{
				$arrayConfig = array();
			}
			$this->m_arrayConfig = $arrayConfig;

			$this->GetWebsite()->LoadStringResources("module-".$this->m_strModuleID);

			$this->m_arrayToolbarConfig = array();

			$this->m_strFinalState = "";
			$this->m_strInitialState = $this->GetState();
			$this->m_strPreviousState = $this->GetModuleVar("previous_state");
			$this->m_bOnInit = false;
			$this->m_bInit = $this->OnInit();
			if ($this->m_bOnInit == false)
			{
				FatalError('Module '.$this->GetModuleID().' must call parent::OnInit()!');
			}
		}

		function GetSatelliteType()
		{
			return 'Module';	
		}


		function OnInit()
		{
			$this->m_bOnInit = true;
			return true;
		}

		final
		function GetInitialState()
		{
			return $this->m_strInitialState;	
		}

		final
		function GetFinalState()
		{
			return $this->m_strFinalState;	
		}
		
		final
		function GetPreviousState()
		{
			return $this->m_strPreviousState;	
		}
		
		final
		function GetState()
		{
			return $this->GetModuleVar("state");
		}
		
		final
		function SetState($strNewState)
		{
			$this->Trace("SetState(\"$strNewState\")");
			$this->SetModuleVar("state",$strNewState);
			$this->SetModuleVar("previous_state",$strNewState);
		}
		
		final
		function GetStyle()
		{
			return $this->m_strStyleID;	
		}
		
		function GetDataFilePath($strFilePath)
		{
			return parent::GetDataFilePath($this->m_strModuleID."/".$strFilePath);
		}
		
		final
		function GetInstanceID()
		{
			return $this->m_strInstanceID;	
		}
		
		final
		function GetModuleID()
		{
			return $this->m_strModuleID;	
		}
		
		final
		function GetModuleDir()
		{
			return $this->m_strModuleDir;	
		}

		final
		function GetModuleVarName($strVar)
		{
			if ($strVar != ")*(")
			{
				// This exception is needed to construct the regular expression in GetAllModuleVars()
				if ($this->GetWebsite()->CheckVarName($strVar) == false)
				{
					$this->Error("GetModuleVarName called with an invalid var name: \"$strVar\". Var names must consist of the following character set: \"".CHARSET_PARAMETERNAME."\"");
				}
			}
			$strInstanceID = ($this->m_strInstanceID == "")?(""):($this->m_strInstanceID."_");
			return $this->GetModuleID()."_".$strInstanceID.$strVar;
		}
		final
		function GetModuleVarNameState()
		{
			return $this->GetModuleVarName("state");	
		}
		final
		function IsModuleVar($strVar)
		{
			return $this->GetWebsite()->IsVar($this->GetModuleVarName($strVar));
		}
		final
		function GetModuleVar($strVar)
		{
			return $this->GetWebsite()->GetVar($this->GetModuleVarName($strVar));
		}
		final
		function GetModuleVarInsecure($strVar)
		{
			return $this->GetWebsite()->GetVarInsecure($this->GetModuleVarName($strVar));
		}		
		final
		function GetAllModuleVars()
		{
			$arrayVars = $this->GetWebsite()->GetAllVars();
			$arrayModuleVars = array();
			foreach ($arrayVars as $strName => $strVar)
			{
				$strRegExp = "/(".$this->GetModuleVarName(")*(").")/";
				if (RegExpMatch($strName,$strRegExp) != false )
				{
					$arrayModuleVars[$strName]	= $strVar;
				}
			}
			return $arrayModuleVars;			
		}
		
		final
		function SetModuleVar($strVar,$strValue)
		{
			$this->GetWebsite()->SetVar($this->GetModuleVarName($strVar),$strValue);
		}

		final
		function IsModuleParam($strParam)
		{
			return $this->IsParam($this->GetModuleVarName($strParam));
		}

		
		final
		function GetModuleParam($strParam)
		{
			return $this->GetParam($this->GetModuleVarName($strParam));
		}
		
		final
		function GetModuleConfig($strConfig1,$strConfig2="",$strConfig3="",$strConfig4="")
		{
			return ArrayGetValue($this->m_arrayConfig,$strConfig1,$strConfig2,$strConfig3,$strConfig4);	
		}
		
		final
		function GetModuleConfigBoolean($strConfig1,$strConfig2="",$strConfig3="",$strConfig4="")
		{
			return GetBoolValue($this->GetModuleConfig($strConfig1,$strConfig2,$strConfig3,$strConfig4));
		}
		
		final
		function GetModuleConfigArray()
		{
			return $this->m_arrayConfig;	
		}
		
		final
		function SetToolbarConfig($arrayToolbarConfig)
		{
			$this->m_arrayToolbarConfig = $arrayToolbarConfig;
		}
		
		final
		function GetToolbarConfig()
		{
			return $this->m_arrayToolbarConfig;
		}
		
		final
		function GetMessage($strMessageType,$strMessageID)
		{
			return $this->GetWebsite()->GetMessage($strMessageType,$strMessageID,StringUpperCase($this->GetModuleID()));
		}
		
		function OnIsToolVisible($strToolID)
		{
			$strState = $this->GetState();
			$this->Trace($strState);
			$arrayStateTools = ArrayGetValue($this->m_arrayToolbarConfig,$strState);
			if (!is_array($arrayStateTools))
			{
				return false;
			}
			if (!ArrayValueExists($arrayStateTools,$strToolID))
			{
				return false;
			}
			return true;
		}
		
		final
		function IsToolVisible($strToolID)
		{
			return $this->OnIsToolVisible($strToolID);
		}


		final
		function LoadModuleLayoutFile($strStateID,$strStyleID)
		{
			// 1. Use the website's assembly line (with style)
			$strFilePath = $this->GetAssemblyLine()->GetModuleLayoutFilePath(
				$this->GetWebsite()->GetLayout(),
				$this->m_strModuleID,
				$strStateID,
				$strStyleID);
			if ($strFilePath != false)
			{
				return FileRead($strFilePath);
			}

			// 2. Use the website's assembly line (without style)
			$strFilePath = $this->GetAssemblyLine()->GetModuleLayoutFilePath(
				$this->GetWebsite()->GetLayout(),
				$this->m_strModuleID,
				$strStateID,
				"");
			if ($strFilePath != false)
			{
				return FileRead($strFilePath);
			}
  
			$this->Error("LoadModuleLayoutFile FAILED: ModuleID: \"".$this->m_strModuleID."\", State: \"$strStateID\", Style: \"$strStyleID\"");
		}

		
		
		final
		function EventHandler($strEventID,$arrayParams)
		{
			if ($strEventID == "website-module-init")
			{
				$this->OnWebsiteInit();	
				return;
			}
			if ($strEventID == "website-user-delete")
			{
				$this->OnUserDelete(ArrayGetValue($arrayParams,"userid"));
				return;
			}
			return parent::EventHandler($strEventID,$arrayParams);
		}
		
		final
		function RenderModule($strStyleID = '')
		{
			$this->Trace('RenderModule BEGIN');
			$this->GetWebsite()->ProfilingTrace();
			$strOutput = "";
			if ($this->m_bInit != true)
			{
				$this->Trace('Module (instance: "'.$this->GetInstanceID().'") was not initialized successfully. Doing nothing.');
			}
			else
			{
				$this->m_strStyleID = $strStyleID;
				$strStateID = $this->OnRenderModule($this->GetState());
				$this->m_strFinalState = $strStateID;
				$strOutput = $this->LoadModuleLayoutFile($strStateID,$strStyleID);
       	$strOutput = $this->ReplaceOutputItems($strOutput);
       	$this->GetWebsite()->SetOutputItems($this->m_arrayOutputItems);
				$this->m_strStyleID = "";
			}
			$this->Trace('RenderModule END');
			$this->GetWebsite()->ProfilingTrace();
			return $strOutput;
		}

		function OnWebsiteInit()
		{
			$this->Trace('OnWebsiteInit is not implemented.');
		}

		function OnUserDelete($strUserID)
		{
			$this->Trace('OnUserDelete is not implemented.');
		}

		
		function OnRenderModule($strState)
		{
			$this->Error('Module '.$this->m_strModuleID.': OnRenderModule not implemented.');
			return '';
		}
		

	}

