<?php

	class CMyContent extends CContent
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->SetFramework("fullwidth");


			// Account Analysis
			$arrayAccount = AML_ReadFileAccount($this->GetWebsite()->GetDataFilePath("mandates/demo/accounts/mockdata-account-frontpage.csv"));
			$arrayAccount['ID'] = '';

			$this->IncludeModule('aml/accountanalysis');
			$arrayConfig = array();
			$arrayConfig['demo'] = true;
			$accountAnalysisModule = new CAccountAnalysisModule($this->GetWebsite(),'start',$arrayConfig);
			$accountAnalysisModule->SetAccount($arrayAccount);
			$this->SetOutputItem('ACCOUNTANALYSIS',$accountAnalysisModule->RenderModule());			


					
			 
			// Network Analysis
			$strMockCSVFilePath = $this->GetWebsite()->GetDataFilePath("mandates/demo/transactions/mockdata-transactions-1.csv");
			$arrayTransactions = FileReadCSV(
				$strMockCSVFilePath,
				true,
				false,
				';',
				array(),
				'"',
				'""');
			// Transform to Accounts data format
			$arrayMapping = array(
				'FROM' => 'ORIGINATOR_CLEAN',
				'TO' => 'BENEFICIARY_CLEAN');
			$arrayAccounts = AML_TransformTransactionsToAccounts(
				$arrayTransactions,
				$arrayMapping);
			// Prepare Network Graph data format
			$arrayNetworkGraph = AML_TransformAccountsToNetworkGraph($arrayAccounts);
			$this->GetWebsite()->IncludeElement('amlnetworkgraph');
			
			/*$amlnetworkgraph = new CAMLNetworkGraphElement($this->GetWebsite(),'default');
			$this->SetOutputItem('NETWORKGRAPH',$amlnetworkgraph->RenderAMLNetworkGraph($arrayNetworkGraph));
			*/
			

			
			
			
		}
	}
	
