<?php


	class CWikiPadModule extends CModule
	{
		private $m_formDef;

		function __construct(
			CWebSite $website,
			$strWikiPageID = 'default',
			$arrayConfig = array())
		{
			parent::__construct(
				$website,
				'cms/wikipad',
				$strWikiPageID,
				$arrayConfig);
		}
		
		function OnInit()
		{
			$bResult = parent::OnInit();

			// Navigation must not resume an editor or replay a saved submission from the session.
			if (!$this->IsModuleParam('state')
				&& in_array($this->GetState(),array('edit','editsection','submit','submitsection'),true))
			{
				$this->SetState('');
				$this->SetModuleVar('content','');
				$this->SetModuleVar('section','');
				$this->SetModuleVar('sectionhash','');
			}

			$this->IncludeElement('wiki');
			$this->IncludeElement('tabnavigation');
			
			if ($this->GetConfigIsUserSpecific() == true)
			{
				if ($this->GetWebsite()->GetAuthUserID() == '')
				{
					return false;	
				}
			}
			
			return $bResult;
		}
		
		

		function OnRenderModule($strState)
		{
			if ($this->IsViewOnly() == true)
			{
				// Request variables survive in the session even when the write is ignored.
				// Do not let a read-only instance seed a later editable instance's draft.
				$this->SetModuleVar('content','');
				$strState = 'view';	
			}
			
			
			switch ($strState)
			{
				case 'submit':
				{
					return $this->StateSubmit();	
				}
				case 'edit':
				{
					return $this->StateEdit();	
				}
				case 'editsection':
					return $this->StateEditSection();
				case 'submitsection':
					return $this->StateSubmitSection();
				case 'view':
				default:	
				{
					return $this->StateView();
				}
			}
		}

		function IsViewOnly()
		{
			return $this->GetModuleConfigBoolean('viewonly');	
		}
		
		function PrepareTabNav($strState)
		{
			if ($this->IsViewOnly() == true)
			{
				$this->SetOutputItem('TABNAV','');
				return;
			}
			
			$tabnavigationDef = new CTabNavigationDefinition();

			if ($this->IsToolVisible('view') == true)
			{
				$arrayParams = array();
	   		$arrayParams[$this->GetModuleVarNameState()] = 'view';
	   		$tabnavigationDef->AddTab(
	      		'view',
	      		'?TID_WIKIPAD_TAB_VIEW?',
	      		$this->GetWebsite()->MakeURL($arrayParams));
			}

			if ($this->IsToolVisible('edit') == true)
			{

				$arrayParams = array();
	   		$arrayParams[$this->GetModuleVarNameState()] = 'edit';
	   		$tabnavigationDef->AddTab(
	      		'edit',
	      		'?TID_WIKIPAD_TAB_EDIT?',
	      		$this->GetWebsite()->MakeURL($arrayParams));
			}

			$tabnav = new CTabNavigationElement(
			   $this->GetWebsite(),
   			'default');

			$this->SetOutputItem('TABNAV',
				$tabnav->RenderTabNavigation(
					$tabnavigationDef,
					$strState));	
		}
		
		function OnIsToolVisible($strToolID)
		{
			return true;	
		}
		
		
		function StateView()
		{
			$this->Trace('StateView');

			$this->PrepareTabNav('view');


			$strContent = $this->GetData($this->GetDataName());
			if ($strContent === false)
			{
				$this->SetErrorMessageCode('WIKIPAD_GET');
				$this->SetOutputItem('WIKI','');
				$strContent = '';
			}

			$wiki = new CWikiElement($this->GetWebSite(),'default');
			$this->ConfigureWikiSectionEditing($wiki,$strContent);
			$this->SetOutputItem(
				'WIKI',
				$wiki->RenderWiki(
					$strContent));

			return 'view';
		}


		function ConfigureWikiSectionEditing($wiki,$strContent)
		{
			if (!$this->IsViewOnly() && $this->IsToolVisible('edit'))
			{
				$nSection = 0;
				$strEditLinkTemplate = null;
				$strSectionToken = uniqid('wdkwikipadsection',false);
				$wiki->SetSectionRenderer(function ($strHtml,$strType) use (&$nSection,&$strEditLinkTemplate,$strSectionToken,$strContent)
				{
					if ($strEditLinkTemplate === null)
					{
						// Routing/session preparation and HTML encoding are identical for every section.
						// Build lazily so pages without boundaries do not need a URL or content hash.
						$strURLTemplate = HtmlEncode($this->GetWebsite()->MakeURL(array(
							$this->GetModuleVarNameState() => 'editsection',
							$this->GetModuleVarName('section') => $strSectionToken,
							$this->GetModuleVarName('sectionhash') => MakeHash($strContent)),false));
						// Avoid a separate whole-page resource replacement for every Edit link.
						$strEditLinkTemplate = $this->GetWebsite()->ReplaceText(
							'<a style="margin-left: auto; font-size: inherit; font-weight: normal;" href="'.
							$strURLTemplate.'">?TID_WIKIPAD_TAB_EDIT?</a>',false);
					}
					$strEditLink = str_replace($strSectionToken,(string)$nSection++,$strEditLinkTemplate);
					if ($strType == 'separator')
					{
						$strHtml = '<div style="flex: 1;">'.$strHtml.'</div>';
					}
					return '<div class="wikipad-section" style="display: flex; align-items: baseline; flex-wrap: wrap; column-gap: 0.4em;">'.
						$strHtml.' '.$strEditLink.'</div>';
				});
			}
		}

		function StateEdit()
		{
			$this->Trace('StateEdit');

			$strContent = $this->GetModuleVarInsecure('content');
			if ($strContent == '')
			{
				$strContent = $this->GetData($this->GetDataName());
				if ($strContent == false)
				{
					$strContent = '';
				}
			}

			return $this->RenderEditForm($strContent);
		}

		function RenderEditForm($strContent,$strSubmitState = 'submit',$arrayParams = array())
		{
			$this->m_formDef = new CFormDefinition();
			$arrayParams[$this->GetModuleVarNameState()] = $strSubmitState;
			$this->m_formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);
			$nLines = intval($this->GetModuleConfig('lines'));
			if ($nLines == 0)
			{
				$nLines = 30;
			}
			$nColumns = $this->GetModuleConfig('columns');
			if ($nColumns == 0)
			{
				$nColumns = 70;
			}

			$strContent = ReplaceString($strContent,'<code>','<CODE>');
			$strContent = ReplaceString($strContent,'</code>','</CODE>');

			$bShowSubmitButtonSaveAndStay = $this->GetModuleConfig('showsubmitbuttonsaveandstay');  

			$bShowSubmitButtonOnTop = $this->GetModuleConfig('showsubmitbuttonontop');
			if ($bShowSubmitButtonOnTop == true)
			{
				$this->m_formDef->AddControlButtonSubmit('?TID_WIKIPAD_BUTTON_SAVE?');
				if ($bShowSubmitButtonSaveAndStay == true)
				{
					$this->m_formDef->AddControlButtonSubmit('?TID_WIKIPAD_BUTTON_SAVE_AND_STAY?','SUBMIT-STAY','stay');
				}
			}
			$this->m_formDef->AddControlTextArea('CONTENT',$this->GetModuleVarName('content'),'',$strContent,$nColumns,$nLines); 
			$this->m_formDef->AddControlButtonSubmit('?TID_WIKIPAD_BUTTON_SAVE?','SUBMIT-2');
			if ($bShowSubmitButtonSaveAndStay == true)
			{
				$this->m_formDef->AddControlButtonSubmit('?TID_WIKIPAD_BUTTON_SAVE_AND_STAY?','SUBMIT-STAY-2','stay');
			}
	
			$form = new CFormElement($this->GetWebsite(),'onecolumn');
			
			$this->SetOutputItem(
				'FORM',
				$form->RenderForm(
					$this->m_formDef,
					array()));
					
			// Section editing must keep the whole-page Edit tab navigable.
			$this->PrepareTabNav($strSubmitState == 'submitsection' ? 'editsection' : 'edit');
	
			return 'edit';
		}

		function MaskWikiLiteralSections($strContent)
		{
			$mask = function ($match) { return preg_replace('/[^\r\n]/',' ',$match[0]); };
			// The renderer extracts syntax-highlight blocks before processing pre blocks.
			$strContent = preg_replace_callback('/<(syntaxhighlight(?:ing)?)\b[^>]*>.*?<\/\1\s*>/is',$mask,$strContent);
			return preg_replace_callback('/<pre>.*?(?:<\/pre>|$)/s',$mask,$strContent);
		}

		function GetSectionBoundaryType($strLine)
		{
			$strLine = trim($strLine);
			if (substr($strLine,0,4) == '----')
			{
				return 'separator';
			}
			// ParseHeaders accepts the highest matching level, capped at six.
			if (substr($strLine,0,1) == '=' && substr($strLine,-1) == '=')
			{
				return 'heading';
			}
			return '';
		}

		// Each boundary starts an independent section, ending at the next boundary.
		function GetEditableSections($strContent)
		{
			$sections = array();
			$nOffset = 0;
			foreach (explode("\n",$this->MaskWikiLiteralSections($strContent)) as $strLine)
			{
				$strType = $this->GetSectionBoundaryType($strLine);
				if ($strType != '')
				{
					$nPrevious = count($sections) - 1;
					if ($nPrevious >= 0)
					{
						$sections[$nPrevious]['length'] = $nOffset - $sections[$nPrevious]['start'];
					}
					$sections[] = array('start' => $nOffset,'length' => strlen($strContent) - $nOffset,'type' => $strType);
				}
				$nOffset += strlen($strLine) + 1;
			}
			return $sections;
		}

		function StateEditSection($bSubmitted = false)
		{
			if ($this->IsViewOnly() || !$this->IsToolVisible('edit'))
			{
				return $this->StateView();
			}
			$strPage = $this->GetData($this->GetDataName());
			$strSection = $this->GetModuleVar('section');
			$sections = $this->GetEditableSections($strPage === false ? '' : $strPage);
			$strRequestedHash = $this->GetModuleVar('sectionhash');
			if (!$bSubmitted && (!ctype_digit((string)$strSection) || !isset($sections[(int)$strSection])
				|| $strPage === false || ($this->IsModuleParam('sectionhash') && $strRequestedHash != ''
					&& $strRequestedHash !== MakeHash($strPage))))
			{
				$this->SetErrorMessageCode('SECTION');
				return $this->StateView();
			}
			$strHash = $bSubmitted ? $strRequestedHash : MakeHash($strPage);
			$strContent = $bSubmitted ? $this->GetModuleVarInsecure('content') :
				substr($strPage,$sections[(int)$strSection]['start'],$sections[(int)$strSection]['length']);
			// Keep a rejected draft in its form without leaking it to whole-page editing.
			$this->SetModuleVar('content','');
			return $this->RenderEditForm($strContent,'submitsection',array(
				$this->GetModuleVarName('section') => HtmlEncode($strSection),
				$this->GetModuleVarName('sectionhash') => HtmlEncode($strHash)));
		}

		function StateSubmitSection()
		{
			if ($this->IsViewOnly() || !$this->IsToolVisible('edit'))
			{
				return $this->StateView();
			}
			$strPage = $this->GetData($this->GetDataName());
			if ($strPage === false)
			{
				$this->SetErrorMessageCode('WIKIPAD_GET');
				return $this->StateEditSection(true);
			}
			$strSection = $this->GetModuleVar('section');
			$sections = $this->GetEditableSections($strPage);
			if (!$this->IsModuleParam('section') || !$this->IsModuleParam('sectionhash') || !$this->IsModuleParam('content')
				|| !ctype_digit((string)$strSection) || !isset($sections[(int)$strSection])
				|| $this->GetModuleVar('sectionhash') !== MakeHash($strPage))
			{
				$this->SetErrorMessageCode('SECTION');
				return $this->StateEditSection(true);
			}
			$section = $sections[(int)$strSection];
			$strContent = $this->GetModuleVarInsecure('content');
			$strContent = ReplaceString($strContent,'<CODE>','<code>');
			$strContent = ReplaceString($strContent,'</CODE>','</code>');
			$strContent = ReplaceString($strContent,u("�"),'--');
			$strSuffix = substr($strPage,$section['start'] + $section['length']);
			if ($strSuffix !== '' && $strContent !== '' && substr($strContent,-1) !== "\n")
			{
				$strContent .= "\n";
			}
			$strNewPage = substr($strPage,0,$section['start']).$strContent.$strSuffix;
			if (!$this->SetData($this->GetDataName(),$strNewPage))
			{
				$this->SetErrorMessageCode('WIKIPAD_SET');
				return $this->StateEditSection(true);
			}
			// Stay only if the edited content still starts with a real boundary.
			$newSections = $this->GetEditableSections($strNewPage);
			$bStay = $this->GetVar('submit') == 'stay' && $strContent !== ''
				&& isset($newSections[(int)$strSection])
				&& $newSections[(int)$strSection]['start'] == $section['start']
				&& $this->GetSectionBoundaryType(strtok($this->MaskWikiLiteralSections($strContent),"\n")) != '';
			$arrayParams = array(
				$this->GetModuleVarNameState() => $bStay ? 'editsection' : 'view',
				$this->GetModuleVarName('section') => $bStay ? $strSection : '',
				$this->GetModuleVarName('sectionhash') => '',
				$this->GetModuleVarName('content') => '',
				$this->GetModuleVarName('action') => '');
			$this->SetModuleVar('content','');
			$this->SetModuleVar('sectionhash','');
			if ($this->RedirectAfterModification($bStay ? 'editsection' : 'view',$arrayParams))
			{
				return false;
			}
			return $bStay ? $this->StateEditSection() : $this->StateView();
		}

		function StateSubmit()
		{
			$this->Trace('StateSubmit');
			
			$this->Trace($this->GetAllModuleVars());
			
			$strContent = $this->GetModuleVarInsecure('content');
			
			$strContent = ReplaceString($strContent,'<CODE>','<code>');
			$strContent = ReplaceString($strContent,'</CODE>','</code>');
			
			$strContent = ReplaceString($strContent,u("�"),'--'); 
			
			$this->SetState('');
		
			if ($this->SetData($this->GetDataName(),$strContent) == false)
			{
				$this->SetErrorMessageCode('WIKIPAD_SET');
				return $this->StateEdit();
			}
			$this->SetModuleVar('content','');
			
			$strSubmitValue = $this->GetVar('submit');
			
			if ($this->RedirectAfterModification($strSubmitValue == 'stay' ? 'edit' : 'view'))
			{
				return false;
			}
			return $strSubmitValue == 'stay' ? $this->StateEdit() : $this->StateView();
		}

		function GetViewStateAfterModification()
		{
			return 'view';
		}

		function RedirectAfterModification($strState = 'view',$arrayParams = array())
		{
			if (!$this->GetWebsite()->IsMobileClient())
			{
				return false;
			}
			// Resolve the destination without loading or rendering the wiki.
			$arrayParams[$this->GetModuleVarNameState()] = $strState == 'view' ? $this->GetViewStateAfterModification() : $strState;
			$arrayParams[$this->GetModuleVarName('action')] = '';
			$this->GetWebsite()->Redirect($this->GetWebsite()->MakeURL($arrayParams));
			return true;
		}
		
		
		function GetConfigIsUserSpecific()
		{
			return $this->GetModuleConfigBoolean('userspecific');	
		}
		
		function GetDataName()
		{
			return 'wikipad_'.$this->GetInstanceID();	
		}



		function GetData($strDataName)
		{
			$strDefaultContent = $this->GetModuleConfig('defaultcontent');
			if ($this->GetConfigIsUserSpecific())
			{
				return $this->GetWebsite()->GetAuthUserData($strDataName,$strDefaultContent);
			}
			else
			{
				return $this->GetWebsite()->GetData('','',$strDataName,$strDefaultContent);
			}
		}

		function SetData($strDataName,$strContent)
		{
			if ($this->GetConfigIsUserSpecific())
			{
				return $this->GetWebsite()->SetAuthUserData($strDataName,$strContent);
			}
			else
			{
				return $this->GetWebsite()->SetData('','',$strDataName,$strContent);
			}
			
		}

	}

