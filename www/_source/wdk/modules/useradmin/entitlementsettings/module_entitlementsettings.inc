<?php

	
	class CEntitlementSettingsModule extends CModule
	{
		protected $m_strUserID;
		protected $m_arrayPrivileges;
		
		function __construct(
			CWebSite $website,
			$strUserID,
			$arrayConfig)
		{
			$this->m_strUserID = $strUserID;
			$strUserID = ArrayGetValue($arrayConfig,"user_id");
			parent::__construct(
				$website,
				"useradmin/entitlementsettings",
				$strUserID,
				$arrayConfig);
		}
		
		function CallbackInit()
		{
			if ($this->m_strUserID == "")
			{
				$this->Error("A user id must be supplied (user_id).");	
			}
			$this->m_arrayPrivileges = $this->GetModuleConfig("privileges");
			if (!is_array($this->m_arrayPrivileges))
			{
				$this->Error("An array of privilege ids must be supplied (privileges).");	
			}
			return true;
		}

		function CallbackRenderModule($strState)
		{
			switch ($strState)
			{
				case "modify-submit":
				{
					return $this->StateModifySubmit();
				}
				case "modify":
				default:
				{
					return $this->StateModify();
				}
			}
		}
		

		
		function StateModify()
		{
			if (ArrayCount($this->m_arrayPrivileges) == 0)
			{
				$this->SetOutputItem("FORM","");
				return "modify";
			}
			
			// Get user's entitlement information
			$webservice = new CWebServiceConsumerWebApplication($this->GetWebsite());
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "get";
			$webservice->ConsumeWebService("system/entitlement",$arrayParams);
			if ($webservice->GetError() != "")
			{
				return "error";	
			}
			$arrayResultList = $webservice->GetResultList();
			
			$arrayUserPrivileges = array();
			foreach ($arrayResultList as $arrayEntry)
			{
				$arrayUserPrivileges[] = $arrayEntry["PRIVILEGE_ID"];
			}
			
			
			// Create Form
			$formDef = new CFormDefinition();
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = "modify-submit";
			$formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);
			
			$formDef->AddControlStatic("TEXT","?TID_ENTITLEMENTSETTINGS_TEXT?");
			
			foreach ($this->m_arrayPrivileges as $strPrivilegeID)
			{
				$bValue = ArrayValueExists($arrayUserPrivileges,$strPrivilegeID);
				$strPrivilegeID = StringUpperCase($strPrivilegeID);
				$formDef->AddControlCheckbox(
					$strPrivilegeID,
					$this->GetModuleVarName(StringLowerCase($strPrivilegeID)),
					"?TID_PRIVILEGE_".$strPrivilegeID."?",
					$bValue);
			}
		
			$formDef->AddControlButtonSubmit("?TID_ENTITLEMENTSETTINGS_BUTTON_EDIT?");
			
			// RenderForm
			$form = new CFormElement($this->GetWebsite(),"default");
			$this->SetOutputItem("FORM", $form->RenderForm($formDef,array()) );
		
			$this->SetModuleVar("state","");
			return "modify";
		}
		
		function StateModifySubmit()
		{
			if (ArrayCount($this->m_arrayPrivileges) == 0)
			{
				$this->SetOutputItem("FORM","");
				return "modify";
			}		
			
		
	
			// Get user's entitlement information
			$webservice = new CWebServiceConsumerWebApplication($this->GetWebsite());
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "get";
			$webservice->ConsumeWebService("system/entitlement",$arrayParams);
			if ($webservice->GetError() != "")
			{
				return "error";
			}
			$arrayResultList = $webservice->GetResultList();
			
			$arrayUserPrivileges = array();
			foreach ($arrayResultList as $arrayEntry)
			{
				$arrayUserPrivileges[] = $arrayEntry["PRIVILEGE_ID"];
			}
			
			
			$bError = false;
			foreach ($this->m_arrayPrivileges as $strPrivilegeID)
			{
				$strPrivilegeID = StringUpperCase($strPrivilegeID);
				
				$bCurrentValue = ArrayValueExists($arrayUserPrivileges,$strPrivilegeID);
				$bNewValue = GetBoolValue($this->GetModuleVar(StringLowerCase($strPrivilegeID)));
				if ($bCurrentValue != $bNewValue)
				{
					$arrayParams = array();
					$arrayParams["user_id"] = $this->m_strUserID;
					if ($bNewValue == true)
					{
						$arrayParams["command"] = "set";
					}
					else
					{
						$arrayParams["command"] = "delete";					
					}
					$arrayParams["privilege_id"] = $strPrivilegeID;
					$webservice->ConsumeWebService("system/entitlement",$arrayParams);
					if ($webservice->GetError() != "")
					{
						$bError = true;
					}
				}
			}
	
			if ($bError == true)
			{
				$this->SetErrorMessageCode("SETPRIVILEGES");
			}
			else
			{
				$this->SetInfoMessageCode("SETPRIVILEGES");
			}
			return $this->StateModify();
		}
		
		
		
		
		
		
		
	}

