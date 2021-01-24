<?php

	class CCaseAnalysisModule extends CAMLModule
	{
		
		private $m_arrayErrors;
		private $m_arrayAccounts;
	
		function __construct(
			CWebSite $website,
			$strInstanceID = '',
			$arrayConfig = array())
		{
			parent::__construct(
				$website,
				'aml/caseanalysis',
				$strInstanceID,
				$arrayConfig);
		}
		
		function OnInit()
		{
			$this->m_arrayErrors = array();
	
			/*		
			$arrayToolbarConfig = array(
				'start' => array('help'),
				'help' => array('start')				
				);
		
			$this->SetToolbarConfig($arrayToolbarConfig);
			*/
			
			$this->m_arrayAccounts = $this->GetAccounts();
			
			return parent::OnInit();
		}
		
	
		function OnRenderModule($strState)
		{
			$this->Trace('OnRenderModule("'.$strState.'")');
			
			$strResult = parent::OnRenderModule($strState);
			if ($strResult != '')
			{
				return $strResult;
			}
	
			
			switch ($strState)
			{
				case 'help':
					return $this->StateHelp();
				case 'export':
					return $this->StateExport();
				case 'exportsubmit':
					return $this->StateExportSubmit();
				case '':
					$this->SetState('start');
				default:
					return $this->StateStart();
			}
		}
		
	
		function RenderToolbar()
		{
			/*
			$toolbarDef = new CToolbarDefinition();

			if ($this->IsToolVisible('help'))
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = 'help';
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, 'help', '?TID_CASEANALYSIS_TOOL_HELP?');
			}


			if ($this->IsToolVisible('start'))
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = 'start';
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, 'list', '?TID_CASEANALYSIS_TOOL_START?');
			}

			$toolbar = new CToolbarElement($this->GetWebsite(),'default');
			$this->SetOutputItem('TOOLBAR',$toolbar->RenderToolbar($toolbarDef)); 
			*/
			$this->SetOutputItem('TOOLBAR',''); 

		}		
		
		function RenderTabNavigation()
		{
/*
			if ($this->IsDemo() == true)
			{
				$this->SetOutputItem('TABNAV','');
   			return;
			}
*/

			$this->IncludeElement('tabnavigation');

			$tabnavigationDef = new CTabNavigationDefinition();
   
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = 'start';
   		$tabnavigationDef->AddTab(
	      'start',
      	'?TID_CASEANALYSIS_TAB_ANALYSIS?',
      	$this->GetWebsite()->MakeURL($arrayParams));

			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = 'export';
   		$tabnavigationDef->AddTab(
	      'export',
      	'?TID_CASEANALYSIS_TAB_EXPORT?',
      	$this->GetWebsite()->MakeURL($arrayParams));

			$tabnavigation = new CTabNavigationElement(
			   $this->GetWebsite(),
			   'default');
			   
			$this->SetOutputItem(
				'TABNAV',
				$tabnavigation->RenderTabNavigation(
   				$tabnavigationDef,
   				$this->GetState()));	
		}
		
		
		function GetAccounts()
		{
			$strAccountDataDir = $this->GetAccountDataDir();
 
 			$arrayFiles = ReadDirectory($strAccountDataDir);
 			
			$arrayAccounts = array();
			
			if (ArrayCount($arrayFiles) != 0)
			{
				foreach ($arrayFiles as $strFile)
				{
					if (GetExtentionFromPath($strFile) != 'csv')
					{
						continue;
					}
	
					$arrayAccount = AML_ReadFileAccount($strAccountDataDir.$strFile);
					if ($arrayAccount == false)
					{
						$this->Trace("AML_ReadFileAccount($strFile) returned false" );
						continue;	
					}
					
					$strID = ArrayGetValue($arrayAccount,'ID');
					if ($strID == '')
					{
						$strID = GetFileTitleFromPath($strFile);
					}
					$arrayAccount['ID'] = $strID;
									
					$arrayAccounts[] = $arrayAccount;
				}
			}		
			
			
			$arrayAccounts = ArraySort($arrayAccounts,false,'CompareAccountsByID');
			
			return $arrayAccounts;			
		}
		
		function StateHelp()
		{
			
			return 'help';
		}		
		
		function StateStart()
		{
			$this->RenderTabNavigation();
			
			$this->GetWebsite()->SetFramework("fullwidth");
			$this->GetWebsite()->MaintainScrollPosition();	

			$arrayAccounts = $this->GetAccounts();

 					
			$strHtml = '';
			
			
	
			if (ArrayCount($arrayAccounts) == 0)
			{
				$message = new CMessageElement($this->GetWebsite(),'info');
				$strHtml = $message->RenderMessage('?TID_CASEANALYSIS_INFO_NODATA?');
			}
			else
			{
				
				//$arrayNetworkGraph = AML_TransformAccountsToNetworkGraph($arrayAccounts);
				
		
				//$strHtml .= '<h1>Network Analysis</h1>'."\n";
				//$this->GetWebsite()->IncludeElement('amlnetworkgraph');
				//$amlnetworkgraph = new CAMLNetworkGraphElement($this->GetWebsite(),'demo');
				//$strHtml .= $amlnetworkgraph->RenderAMLNetworkGraph($arrayNetworkGraph);
				$this->IncludeModule('aml/accountanalysis');
				$nIndex = 0;
				foreach ($arrayAccounts as $arrayAccount)
				{
					$accountAnalysisModule = new CAccountAnalysisModule($this->GetWebsite(),'account'.$nIndex);
					$accountAnalysisModule->SetAccount($arrayAccount);
					$strHtml .= $accountAnalysisModule->RenderModule();
					$nIndex++;
				}	
				
			}
			
			
			$this->SetOutputItem('CASEANALYSIS',$strHtml);
	
			
			return 'start';
		}
		

		function StateExport()
		{
			$this->SetState('export');
			$this->RenderTabNavigation();
			
			$arrayAccounts = $this->GetAccounts();
			if (ArrayCount($arrayAccounts) == 0)
			{
				$message = new CMessageElement($this->GetWebsite(),'info');
				$this->SetOutputItem('FORM',$message->RenderMessage('?TID_CASEANALYSIS_INFO_NODATA?'));
			}
			
			
			
			$formDef = new CFormDefinition();
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = 'exportsubmit';		

			$formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);

			$formDef->AddControlStatic('TEXT','?TID_CASEANALYSIS_TEXT_EXPORT?');
	
			$formDef->AddControlButtonSubmit('?TID_CASEANALYSIS_BUTTON_EXPORT?');
			
			$form = new CFormElement($this->GetWebsite(),'default');
			
			$this->SetOutputItem( 
				'FORM',
				$form->RenderForm(
					$formDef,
					array(),					
					$this->m_arrayErrors));
		
			return 'export';			
		}
		
		
		function StateExportSubmit()
		{
			$arrayAccounts = $this->GetAccounts();
			
			if (ArrayCount($arrayAccounts) == 0)
			{
				return $this->StateExport();
			}		
	
			
			// Move this into a AML_* function			
			$arrayRiskCountries = AML_GetRiskCountries();
			$arrayTypologyIDs = AML_GetSupportedTypologyIDs();

			// Inactivities cannot be exported and hence do not have to be raised here.
			$arrayTypologyIDs = ArrayRemoveValue($arrayTypologyIDs,'INACTIVITY');
			
			$arrayTypologyDefinitions = AML_GetDefaultTypologyDefinitions($arrayTypologyIDs);
			if (ArrayCount($arrayRiskCountries) > 0)
			{
				$arrayTypologyDefinitions['HIGHRISKGEOGRAPHIES']['COUNTRIES'] = $arrayRiskCountries;
			}

			$arrayAllTransactions = array();
			foreach ($arrayAccounts as $arrayAccount)  
			{
				$strAccountID = ArrayGetValue($arrayAccount,'ID');
				$arrayRedFlags = array();
				$arrayRedFlags = AML_RaiseRedFlags($arrayAccount,$arrayTypologyDefinitions);
				
				$arrayTransactions = array();
				foreach ($arrayRedFlags as $arrayRedFlag)
				{
					$arrayRedFlagTransactions = ArrayGetValue($arrayRedFlag,'TRANSACTIONS');
					$strTypology = ArrayGetValue($arrayRedFlag,'TYPOLOGY');
					if (ArrayCount($arrayRedFlagTransactions) > 0)
					{
						foreach ($arrayRedFlagTransactions as $arrayRedFlagTransaction)
						{
							// TOOD: Move this into an AML* function
							$strTransactionID =
								ArrayGetValue($arrayRedFlagTransaction,'DATE')
								.ArrayGetValue($arrayRedFlagTransaction,'ID')
								.ArrayGetValue($arrayRedFlagTransaction,'AMOUNT')
								.ArrayGetValue($arrayRedFlagTransaction,'TYPE')
								.ArrayGetValue($arrayRedFlagTransaction,'COUNTRY')
								.ArrayGetValue($arrayRedFlagTransaction,'DESCRIPTION');
							
							$arrayTransactions[$strTransactionID]['ACCOUNTID'] = $strAccountID;
							$arrayTransactions[$strTransactionID]['DATE'] = ArrayGetValue($arrayRedFlagTransaction,'DATE');
							$arrayTransactions[$strTransactionID]['ID'] = ArrayGetValue($arrayRedFlagTransaction,'ID');
							$arrayTransactions[$strTransactionID]['AMOUNT'] = ArrayGetValue($arrayRedFlagTransaction,'AMOUNT');
							$arrayTransactions[$strTransactionID]['TYPE'] = ArrayGetValue($arrayRedFlagTransaction,'TYPE');
							$arrayTransactions[$strTransactionID]['COUNTRY'] = ArrayGetValue($arrayRedFlagTransaction,'COUNTRY');
							$arrayTransactions[$strTransactionID]['DESCRIPTION'] = ArrayGetValue($arrayRedFlagTransaction,'DESCRIPTION');
							$arrayTransactions[$strTransactionID][$strTypology] = true;

						}
					}
				}
				$arrayTransactions = array_values($arrayTransactions);
				$arrayTransactions = ArraySort($arrayTransactions,false,'CompareTransactionByDate');
				
				$arrayAllTransactions = ArrayAppend($arrayAllTransactions,$arrayTransactions);
			}
						 
	
			
			$strCSV = $this->GetWebsite()->MakeCSV($arrayAllTransactions);
			
			$strFileName = 'suspicioustransactions.csv'; 
			
			$this->GetWebsite()->DownloadFile(
				'text/csv',
				$strFileName,
				$strCSV);			
			
			return $this->StateExport();			
		}

	


	}

