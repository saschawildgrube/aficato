<?php



	class CXXXModule extends CModule
	{
	
		function __construct(
			$website)
		{
			parent::__construct(
				$website,
				"yyy/xxx");
		}
		
		function CallbackInit()
		{
			if ($this->GetState() == "")
			{				
				$this->SetState("list");	
			}
			
			$arrayToolbarConfig = array(
				"state1" => array("tool1"),
				"state2" => array("tool1","tool2")
				);
			
			$this->SetToolbarConfig($arrayToolbarConfig);
			
			return true;
		}

		function CallbackRenderModule($strState)
		{
			switch ($strState)
			{
				case "state1":
				{
					return $this->StateState1();
				}				
				case "state1_submit":
				{
					return $this->StateState1Submit();
				}				
				case "state2":
				{
					return $this->StateState2();
				}				
				default:
				{
					return $this->StateState1();
				}
			}
		}
		
		function RenderToolbar()
		{
			$toolbarDef = new CToolbarDefinition();


			if ($this->IsToolVisible("tool1"))
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "state2";
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, "url", "?TID_XXX_TOOL_TOOL1?");
			}


			if ($this->IsToolVisible("tool2"))
			{
			
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "state1";
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, "url", "?TID_XXX_TOOL_TOOL2?");
			}

			
			
			
			$toolbar = new CToolbarElement($this->GetWebsite(),"default");
			$this->SetOutputItem("TOOLBAR",$toolbar->RenderToolbar($toolbarDef)); 
		}
		
		function StateState1()
		{
			$this->SetState("state1");
			
			
	
	
			// TOOLBAR
			$this->RenderToolbar();

			// FORM
			
			$formDef = new CFormDefinition();
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = "state1-submit";
			$formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);
			
			$formDef->AddControlText("VALUE1",$this->GetModuleVarName("value1"),"Value 1",$this->GetModuleVar("value1"),40,40);
			$formDef->AddControlTextArea("VALUE2",$this->GetModuleVarName("value2"),"Value 2",$this->GetModuleVar("value2"),50,10);
			
			$formDef->AddControlButtonSubmit("Submit");
			
			$form = new CFormElement($this->GetWebsite(),"default");
			
			$this->SetOutputItem("FORM",$form->RenderForm($formDef,array()));
			
			return "state1";
		}

		
		function StateState1Submit()
		{
			$this->SetState("state1-submit");
			$strValue1 = $this->GetModuleVar("value1");
			$strValue2 = $this->GetModuleVar("value2");
			
			
	
			$this->SetModuleVar("value1","");
			$this->SetModuleVar("value2","");
			
			return "state2-submit";
		}

	
		
	}

