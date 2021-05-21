<?php


	class CMyContent extends CContent
	{
		function OnRenderContent(&$arrayItems)
		{

			$table = new CTableElement($this->GetWebsite(),'default');
			$this->GetWebsite()->IncludeElement('format');
			$formatStrong = new CFormatElement($this->GetWebsite(),'strong');


			// Read data from csv

			$strMockCSVFilePath = $this->GetWebsite()->GetDataFilePath('/mandates/demo/transactions/mockdata-transactions-1.csv');
			$arrayTransactions = FileReadCSV(
				$strMockCSVFilePath,
				true,
				false,
				';',
				array(),
				''',
				'''');
			$strHtml = $table->RenderTable($arrayTransactions);
			$this->SetOutputItem('DATA_TRANSACTIONS',$strHtml);
			
			
			
			// Transform to Accounts data format
			$arrayMapping = array(
				'FROM' => 'ORIGINATOR_CLEAN',
				'TO' => 'BENEFICIARY_CLEAN');
			$arrayAccounts = AML_TransformTransactionsToAccounts(
				$arrayTransactions,
				$arrayMapping);
				
			$strHtml = '';
			foreach ($arrayAccounts as &$arrayAccount)
			{
				$arrayAccount['CURRENCY'] = 'EUR';
				
				$strHtml .= $formatStrong->RenderFormat(ArrayGetValue($arrayAccount,'ID'));
				$strHtml .= $table->RenderTable(ArrayGetValue($arrayAccount,'TRANSACTIONS'));
			}
			
			$arrayAccounts2 = AML_CalculateAccounts($arrayAccounts);
			$strHtml .= '<pre>'.ArrayRender($arrayAccounts2).'</pre>';
			$this->SetOutputItem('DATA_ACCOUNTS',$strHtml);
			
			


				
			
		}
	}
	
