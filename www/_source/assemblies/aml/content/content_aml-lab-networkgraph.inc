<?php


	class CMyContent extends CContent
	{
		function OnRenderContent(&$arrayItems)
		{
			// Read data from csv
			$strMockCSVFilePath = $this->GetWebsite()->GetDataFilePath('/mandates/demo/transactions/mockdata-transactions-1.csv');
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
			foreach ($arrayAccounts as &$arrayAccount)
			{
				$arrayAccount['CURRENCY'] = 'EUR';
			}
			$strHtml = '<pre>'.ArrayRender($arrayAccounts).'</pre>';
			$this->SetOutputItem('DATA_ACCOUNTS',$strHtml);
			
			
			// Prepare Network Graph data format
			$arrayNetworkGraph = AML_TransformAccountsToNetworkGraph($arrayAccounts);
			$strHtml = '<pre>'.ArrayRender($arrayNetworkGraph).'</pre>';
			$this->SetOutputItem('DATA_NETWORKGRAPH',$strHtml);
			
			
			$this->GetWebsite()->IncludeElement('amlnetworkgraph');
			$amlnetworkgraph = new CAMLNetworkGraphElement($this->GetWebsite(),'default');
			$this->SetOutputItem('NETWORKGRAPH',$amlnetworkgraph->RenderAMLNetworkGraph($arrayNetworkGraph));
				
			
		}
	}
	
