<?php

	require_once(GetWDKDir().'wdk_module.inc');
	
	class C$$$ModuleID$$$Module extends CModule
	{
		
		private $m_arrayErrors;
	
		function __construct(
			CWebSite $website,
			$strInstanceID = '',
			$arrayConfig = array())
		{
			parent::__construct(
				$website,
				'$$$modulegroup$$$/$$$moduleid$$$',
				$strInstanceID,
				$arrayConfig);
		}
		
		function OnInit()
		{
			$this->m_arrayErrors = array();
			
			$arrayToolbarConfig = array(
				'start' => array('help'),
				'help' => array('start'),				
				'action1' => array('help','start'),
				'action1submit' => array('help','start','action1'),
				'action2' => array('help','start'),
				'action2submit' => array('help','start','action2')
				);
			
			$this->SetToolbarConfig($arrayToolbarConfig);
			
			return parent::OnInit();
		}
		
	
		function OnRenderModule($strState)
		{
			$this->Trace('OnRenderModule("'.$strState.'")');
			switch ($strState)
			{
				case 'help':
					return $this->StateHelp();
				case 'action1':
					return $this->StateAction1();
				case 'action1submit':
					return $this->StateAction1Submit();
				case 'action2':
					return $this->StateAction2();
				case 'action2submit':
					return $this->StateAction2Submit();
				case '':
					$this->SetState('start');
				default:
					return $this->StateStart();
			}
		}
		
	
		function RenderToolbar()
		{
			$toolbarDef = new CToolbarDefinition();

			if ($this->IsToolVisible('help'))
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = 'help';
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, 'help', '?TID_$$$MODULEID$$$_TOOL_HELP?');
			}


			if ($this->IsToolVisible('start'))
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = 'start';
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, 'list', '?TID_$$$MODULEID$$$_TOOL_START?');
			}

			if ($this->IsToolVisible('action1'))
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = 'action1';
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, 'execute', '?TID_$$$MODULEID$$$_TOOL_ACTION1?');
			}

			if ($this->IsToolVisible('action2'))
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = 'action2';
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, 'execute', '?TID_$$$MODULEID$$$_TOOL_ACTION2?');
			}


			$toolbar = new CToolbarElement($this->GetWebsite(),'default');
			$this->SetOutputItem('TOOLBAR',$toolbar->RenderToolbar($toolbarDef)); 
		}		
		
		function StateHelp()
		{
			$this->RenderToolBar();
			
			return 'help';
		}		
		
		function StateStart()
		{
			$this->RenderToolBar();
			
			$link = new CLinkElement($this->GetWebsite(),'default');
			$list = new CListElement($this->GetWebsite(),'default');
			
			$arrayList = array();

			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = 'action1';
			$strLink = $this->GetWebsite()->MakeURL($arrayParams);
			$arrayList[] = $link->RenderLink($this->GetWebsite()->MakeURL($arrayParams),'?TID_$$$MODULEID$$$_TOOL_ACTION1?');

			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = 'action2';
			$strLink = $this->GetWebsite()->MakeURL($arrayParams);
			$arrayList[] = $link->RenderLink($this->GetWebsite()->MakeURL($arrayParams),'?TID_$$$MODULEID$$$_TOOL_ACTION2?');

			
			$this->SetOutputItem('LIST',$list->RenderList($arrayList));
			
			return 'start';
		}
		
		function StateAction1()
		{
			$formDef = new CFormDefinition();
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = 'action1submit';		

			$formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);

			$strValue = $this->GetModuleVar('value1');

			$formDef->AddControlStatic('STATIC1','?TID_$$$MODULEID$$$_TEXT_ACTION1?');
			$formDef->AddControlText('VALUE',$this->GetModuleVarName('value1'),'?TID_$$$MODULEID$$$_LABEL_VALUE?',$strValue,50,255);
			$formDef->AddControlButtonSubmit('?TID_$$$MODULEID$$$_BUTTON_SUBMIT?');
			
			$form = new CFormElement($this->GetWebsite(),'default');
			
			$this->SetOutputItem( 
				'FORM',
				$form->RenderForm(
					$formDef,
					array(),					
					$this->m_arrayErrors));
					
			$this->RenderToolBar();
			
			return 'action1';
		}
		
		function StateAction1Submit()
		{
			$strValue = $this->GetModuleVar('value1');
			if ($strValue == '')
			{
				$this->m_arrayErrors['VALUE'] = '?TID_$$$MODULEID$$$_ERROR_VALUE_INVALID?';	
			}
			
			if (ArrayCount($this->m_arrayErrors) > 0)			
			{
				return $this->StateAction1();	
			}
			
			$this->SetOutputItem('VALUE',$strValue);
			
			$message = new CMessageElement($this->GetWebsite(),'info');
			$this->SetOutputItem('MESSAGE',$message->RenderMessage('?TID_$$$MODULEID$$$_INFO_SUCCESS?'));
			
			$this->SetModuleVar('value1','');
			
			$this->RenderToolBar();
			
			return 'action1submit';
		}
		

		function StateAction2()
		{
			$formDef = new CFormDefinition();
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = 'action2submit';		

			$formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);

			$strValue = $this->GetModuleVar('value2');

			$formDef->AddControlStatic('STATIC1','?TID_$$$MODULEID$$$_TEXT_ACTION2?');
			$formDef->AddControlText('VALUE',$this->GetModuleVarName('value2'),'?TID_$$$MODULEID$$$_LABEL_VALUE?',$strValue,50,255);
			$formDef->AddControlButtonSubmit('?TID_$$$MODULEID$$$_BUTTON_SUBMIT?');
			
			$form = new CFormElement($this->GetWebsite(),'default');
			
			$this->SetOutputItem( 
				'FORM',
				$form->RenderForm(
					$formDef,
					array(),	
					$this->m_arrayErrors));
					
			$this->RenderToolBar();
			
			return 'action2';
		}

		function StateAction2Submit()
		{
			$strValue = $this->GetModuleVar('value2');
			if ($strValue == '')
			{
				$this->m_arrayErrors['VALUE'] = '?TID_$$$MODULEID$$$_ERROR_VALUE_INVALID?';	
			}
			
			if (ArrayCount($this->m_arrayErrors) > 0)			
			{
				return $this->StateAction2();	
			}
			
			$this->SetOutputItem('VALUE',$strValue);
			
			$message = new CMessageElement($this->GetWebsite(),'info');
			$this->SetOutputItem('MESSAGE',$message->RenderMessage('?TID_$$$MODULEID$$$_INFO_SUCCESS?'));
			
			$this->SetModuleVar('value2','');
			
			$this->RenderToolBar();
			
			return 'action2submit';
		}


	}

