<?php




	class CMyContent extends CContent
	{
		
		function OnRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->SetFramework("fullwidth");
 		
			$strFilePath = $this->GetDataFilePath('mandates/tour/accounts/tour.csv');  
			$arrayAccount = AML_ReadFileAccount($strFilePath);
			
			$arrayAccounts = array();
			$arrayAccounts[] = $arrayAccount;
				
			$arrayNetworkGraph = AML_TransformAccountsToNetworkGraph($arrayAccounts);
			$this->GetWebsite()->IncludeElement('amlnetworkgraph');
			$amlnetworkgraph = new CAMLNetworkGraphElement($this->GetWebsite(),'demo');
			$amlnetworkgraph->RenderAMLNetworkGraph($arrayNetworkGraph);
				
			$arrayItems["NETWORK"] = $amlnetworkgraph->RenderAMLNetworkGraph($arrayNetworkGraph);

			$link = new CLinkElement($this->GetWebsite(),'calltoaction');
			$arrayItems["NEXT"] = $link->RenderLink($this->GetWebsite()->MakeContentURL('tour4'),'?TID_BUTTON_NEXT?');
			
		}
	}
	
