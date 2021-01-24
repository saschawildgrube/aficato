<?php


	class CMyContent extends CContent
	{
		function OnRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->SetFramework('blank');
			
			$this->GetWebsite()->AddJavaScript('d3_aml-networkgraph');
			$this->GetWebsite()->AddStyleSheet('d3_aml-networkgraph');			 



			$strMockCSVFilePath = $this->GetWebsite()->GetDataFilePath("mockdata-transactions-1.csv");
			$arrayTransactions = FileReadCSV(
				$strMockCSVFilePath,
				true,
				false,
				';',
				array(),
				'"',
				'""');
			
			$arrayMapping = array(
				'FROM' => 'ORIGINATOR_CLEAN',
				'TO' => 'BENEFICIARY_CLEAN');
			$arrayAccounts = AML_TransformTransactionsToAccounts(
				$arrayTransactions,
				$arrayMapping);
			
			// Randomly set the data quality to 0
			foreach ($arrayAccounts as &$arrayAccount)
			{
				$arrayAccount['DATAQUALITY'] = GetRandomInteger(1);
				$arrayAccount['CURRENCY'] = 'EUR';
			}
			
			
			$arrayNetwork = AML_TransformAccountsToNetworkGraph($arrayAccounts);
			$strJson = json_encode($arrayNetwork,JSON_PRETTY_PRINT);
			$strJavaScript = 'aDataNetwork = '.$strJson.';';
			$strJavaScript .= "AML_NetworkGraph('#aml-demo-networkgraph',aDataNetwork);";
			$this->GetWebsite()->AddJavaScriptSourceCode('aml-demo-networkgraph',$strJavaScript);
			
			
				
			
		}
	}
	
