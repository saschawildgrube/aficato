<?php

	require_once(GetWDKDir().'wdk_httpupload.inc');
	
	class CAccountDataManagerModule extends CAMLModule
	{
		
		private $m_arrayErrors;
		private $m_strMandateID;
	
		function __construct(
			CWebSite $website,
			$strInstanceID = '',
			$arrayConfig = array())
		{
			parent::__construct(
				$website,
				'aml/accountdatamanager',
				$strInstanceID,
				$arrayConfig);
		}
		
		function CallbackInit()
		{
			$this->m_arrayErrors = array();
			$this->m_strMandateID = false;
			
			$arrayToolbarConfig = array(
				'list' => array(/*'help',*/'upload'),
				'help' => array('list','upload'),				
				'upload' => array(/*'help',*/'list'),
				'uploadsubmit' => array(/*'help',*/'list','upload'),
				'deletesubmit' => array(/*'help',*/'list')
				);
			
			$this->SetToolbarConfig($arrayToolbarConfig);
			
			return true;
		}
		
	
		function CallbackRenderModule($strState)
		{
			$this->Trace("CallbackRenderModule(\"$strState\")");
			
			$strResult = parent::CallbackRenderModule($strState);
			if ($strResult != '')
			{
				return $strResult;
			}
			
			switch ($strState)
			{
				case 'help':
					return $this->StateHelp();
				case 'upload':
					return $this->StateUpload();
				case 'uploadsubmit':
					return $this->StateUploadSubmit();
				case 'delete':
					return $this->StateDelete();
				case 'deletesubmit':
					return $this->StateDeleteSubmit();
				case '':
					$this->SetState('list');
				default:
					return $this->StateList();
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
				$toolbarDef->AddTool($strLink, 'help', '?TID_ACCOUNTDATAMANAGER_TOOL_HELP?');
			}

			if ($this->IsToolVisible('list'))
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = 'list';
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, 'list', '?TID_ACCOUNTDATAMANAGER_TOOL_LIST?');
			}

			if ($this->IsToolVisible('upload'))
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = 'upload';
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, 'execute', '?TID_ACCOUNTDATAMANAGER_TOOL_UPLOAD?');
			}

			$toolbar = new CToolbarElement($this->GetWebsite(),'default');
			$this->SetOutputItem('TOOLBAR',$toolbar->RenderToolbar($toolbarDef)); 
		}		
		
		function StateHelp()
		{
			$this->RenderToolBar();
			
			return 'help';
		}		
		
		function StateList()
		{
			$this->RenderToolBar();
			
			$arrayAccountDataFiles = $this->GetAccountDataFiles();
			
			if (ArrayCount($arrayAccountDataFiles) == 0)
			{
				$this->SetState('upload');
				return $this->StateUpload();	
			}
		
			$linkicon = new CLinkIconElement($this->GetWebsite(),"default");
			
			$this->IncludeElement("itemmanager");
			$itemmanagerdata = new CItemManagerData();
			
			$arrayTable = array();
			foreach ($arrayAccountDataFiles as $strFilePath)
			{
				$strFileTitle = GetFileTitleFromPath($strFilePath);
				$strFileName = GetFileNameFromPath($strFilePath);

				$strTooltip = '?TID_ACCOUNTDATAMANAGER_TOOLTIP_DELETE?';

				$arrayParams = array();	
				$arrayParams[$this->GetModuleVarNameState()] = 'delete';
				$arrayParams[$this->GetModuleVarName('filename')] = $strFileName;
				$strTaskURL = $this->GetWebsite()->MakeURL($arrayParams);

				$strDeleteLinkIcon = $linkicon->RenderLinkIcon(
					$strTaskURL,
					'delete',
					$strTooltip);							
				

				$arrayTable[] = array(
					'FILENAME' => $strFileName,
					'TASKS' => $strDeleteLinkIcon
					);
			}
			
			
			$itemmanagerdata->SetItemList($arrayTable);
	
/*			
			$arrayFilterOptionTabs = array(
			   "all" => "All",
			   "even" => "Even",
			   "uneven" => "Uneven"
			   );
			$itemmanagerdata->SetFilterOptionsTabNavigation(
			   $strFilterOptionTab,
			   "itemmanager_filteroptions_tab",
			   $arrayFilterOptionTabs);
*/			
			
			
			$itemmanagerdata->AddColumn('FILENAME','?TID_ACCOUNTDATAMANAGER_TABLEHEADER_FILENAME?','',false);
			$itemmanagerdata->AddColumn('TASKS','?TID_ACCOUNTDATAMANAGER_TABLEHEADER_TASKS?','',false);

			
/*			 
			$itemmanagerdata->SetSortOptions(
			   "itemmanager_sort_option",
			   $this->GetVar("itemmanager_sort_option"),
			   "itemmanager_sort_order",
			   $this->GetVar("itemmanager_sort_order")
			   );
*/			
			   
/*			
			$itemmanagerdata->SetPageNavigation(
			   $this->GetWebsite()->GetVar("itemmanager_offset"),
			   10,
			   "itemmanager_offset",
			   20);
*/
			
			$itemmanager = new CItemManagerElement($this->GetWebsite(),"default");
			$this->SetOutputItem('LIST',$itemmanager->RenderItemManager($itemmanagerdata));			

			
			return 'list';
		}
		
		function StateUpload()
		{
			$this->RenderToolBar();			
			
			$formDef = new CFormDefinition();
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = 'uploadsubmit';		

			$formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);

			$formDef->AddControlStatic('TEXT','?TID_ACCOUNTDATAMANAGER_TEXT_UPLOAD?');
			
			$formDef->AddControlFile(
				'FILE',
				$this->GetModuleVarName('file'),
   			'?TID_ACCOUNTDATAMANAGER_LABEL_FILE?'); 
	
			$formDef->AddControlButtonSubmit('?TID_ACCOUNTDATAMANAGER_BUTTON_UPLOAD?');
			
			$form = new CFormElement($this->GetWebsite(),'default');
			
			$this->SetOutputItem( 
				'FORM',
				$form->RenderForm(
					$formDef,
					array(),					
					$this->m_arrayErrors));
					
	
			return 'upload';
		}
		
		function StateUploadSubmit()
		{
			$strFileID = $this->GetModuleVarName('file');
			if ($strFileID == '')
			{
				$this->m_arrayErrors['FILE'] = '?TID_ACCOUNTDATAMANAGER_ERROR_NOFILE?';
				return $this->StateUpload();	
			}			
			
			$strFilePath = GetUploadedFilePath($strFileID);
			
			if (IsFile($strFilePath) == false)
			{
				$this->m_arrayErrors['FILE'] = '?TID_ACCOUNTDATAMANAGER_ERROR_NOFILE?';
				return $this->StateUpload();	
			}
	
			$strFileName = GetUploadedFileName($strFileID);
			$strExtension = GetExtentionFromPath($strFileName);
			$strExtension = StringLowerCase($strExtension);
			
		
			if (StringLowerCase($strExtension) != 'csv')
			{
				$this->m_arrayErrors['FILE'] = '?TID_ACCOUNTDATAMANAGER_ERROR_FILENOTSUPPORTED?';
				return $this->StateUpload();	
			}



			$arrayAccount = AML_ReadFileAccount($strFilePath);
			if ($arrayAccount == false)
			{
				$this->m_arrayErrors['FILE'] = '?TID_ACCOUNTDATAMANAGER_ERROR_FILENOTLOADED?';
				return $this->StateUpload();	
			}
 
			$strFileContents = FileRead($strFilePath);
			
			$strTargetFilePath = $this->GetAccountDataDir().$strFileName;
			
			$bResult = FileWrite($strTargetFilePath,$strFileContents,false);
			if ($bResult == true)
			{
				$message = new CMessageElement($this->GetWebsite(),'info');
				$this->SetOutputItem('MESSAGE',$message->RenderMessage('?TID_ACCOUNTDATAMANAGER_INFO_UPLOADSUCCESS?'));
			}
			else
			{				
				$bResult = FileWrite($strTargetFilePath,$strFileContents,true);
				if ($bResult == true)
				{
					$message = new CMessageElement($this->GetWebsite(),'info');
					$this->SetOutputItem('MESSAGE',$message->RenderMessage('?TID_ACCOUNTDATAMANAGER_INFO_UPLOADSUCCESSOVERWRITE?'));
				}
				else
				{
					$this->m_arrayErrors['FILE'] = '?TID_ACCOUNTDATAMANAGER_ERROR_FILENOTSTORED?';
					return $this->StateUpload();	
				}
			}	
			
			$this->RenderToolBar();
			
			$this->SetModuleVar('file','');
			$this->SetState('list');			
			
			return 'uploadsubmit';
		}
		

		function StateDelete()
		{
		
			$strFileName = $this->GetModuleVar('filename');
			if ($strFileName == '')
			{
				return $this->StateList();	
			}
		
			$arrayAccountDataFiles = $this->GetAccountDataFiles();
			
			if (ArrayValueExists($arrayAccountDataFiles,$strFileName) == false)
			{
				$this->GetWebsite()->SetOutputItem('FILETODELETE',$strFileName);
				$this->SetErrorMessageCode('NOFILETODELETE');
				return $this->StateList();	
			}
			
			
			$formDef = new CFormDefinition();
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = 'deletesubmit';		

			$formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);

			$this->SetOutputItem('FILETODELETE',$strFileName);
			$formDef->AddControlStatic('STATIC1','?TID_ACCOUNTDATAMANAGER_TEXT_DELETE?');
			$formDef->AddControlButtonSubmit('?TID_ACCOUNTDATAMANAGER_BUTTON_DELETE?');
			
			$form = new CFormElement($this->GetWebsite(),'default');
			
			$this->SetOutputItem( 
				'FORM',
				$form->RenderForm(
					$formDef,
					array(),	
					$this->m_arrayErrors));
					
			$this->RenderToolBar();
			
			return 'delete';
		}

		function StateDeleteSubmit()
		{
			
			$strFileName = $this->GetModuleVar('filename');
			if ($strFileName == '')
			{
				return $this->StateList();
			}

			$arrayAccountDataFiles = $this->GetAccountDataFiles();
			
			if (ArrayValueExists($arrayAccountDataFiles,$strFileName) == false)
			{
				$this->GetWebsite()->SetOutputItem('FILETODELETE',$strFileName);
				$this->SetErrorMessageCode('NOFILETODELETE');
				return $this->StateList();	
			}
			
			$strFilePath = $this->GetAccountDataDir().$strFileName;
			
			if (DeleteFile($strFilePath) == false)
			{
				$this->GetWebsite()->SetOutputItem('FILETODELETE',$strFileName);
				$this->SetErrorMessageCode('DELETEFAILED');
				return $this->StateList();	
				
			}
			
			$message = new CMessageElement($this->GetWebsite(),'info');
			$this->SetOutputItem('MESSAGE',$message->RenderMessage('?TID_ACCOUNTDATAMANAGER_INFO_DELETESUCCESS?'));
			
			$this->RenderToolBar();


			$this->SetModuleVar('filename','');
			$this->SetState('list');
						
			return 'deletesubmit';
		}
		

	}

