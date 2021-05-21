<?php


	class CMyContent extends CContent
	{
		function OnRenderContent(&$arrayItems)
		{
			
			$strMockCSVFilePath = $this->GetWebsite()->GetDataFilePath('/mandates/demo/accounts/mockdata-account-a-transactions.csv');
			$arrayTransactions = FileReadCSV(
				$strMockCSVFilePath,
				true,
				false,
				';',
				array(),
				''',
				'''');
			$arrayAccount = array();
			$arrayAccount['ID'] = 'A';
			$arrayAccount['BALANCE_BEGIN'] = 3330.0;
			$arrayAccount['CURRENCY'] = 'USD';
			$arrayAccount['TRANSACTIONS'] = $arrayTransactions;

			$strHtml = '<pre>'.ArrayRender($arrayAccount).'</pre>';
			$this->SetOutputItem('DATA_ACCOUNT',$strHtml);

			
			$arrayAccountChart = AML_TransformAccountToAccountChart($arrayAccount,'2019-01-01','2019-05-30','MONTHLY',AML_GetDefaultTransactionTypes()); 
			$strHtml = '<pre>'.ArrayRender($arrayAccountChart).'</pre>';
			$this->SetOutputItem('DATA_ACCOUNTCHART',$strHtml);

/*
			$strJson = json_encode($arrayAccountChart,JSON_PRETTY_PRINT);
			$strJavaScript = 'aDataAccountChart = '.$strJson.';';
			$strJavaScript .= 'aPropsAccountChart = [ 'data': aDataAccountChart ];';
			$strJavaScript .= 'D3_AML_AccountChart('#aml-accountchart',aPropsAccountChart);'; 
			$this->GetWebsite()->AddJavaScriptSourceCode('aml-lab-accountchart',$strJavaScript);
			
			$this->GetWebsite()->IncludeElement('amlaccountchartlegend');
			$amlaccountchartlegend = new CAMLAccountChartLegendElement($this->GetWebsite(),'default');
			$this->GetWebsite()->SetOutputItem('ACCOUNTCHARTLEGEND',$amlaccountchartlegend->RenderAMLAccountChartLegend());
	*/		
			
			$this->GetWebsite()->IncludeElement('amlaccountchart');
			$amlaccountchart = new CAMLAccountChartElement($this->GetWebsite(),'default');
			$this->GetWebsite()->SetOutputItem('ACCOUNTCHART',$amlaccountchart->RenderAMLAccountChart($arrayAccountChart));
			
			
			
			
			
		}
	}
	
