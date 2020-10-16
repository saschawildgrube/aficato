<?php




	class CMyContent extends CContent
	{
		function CallbackRenderSpecialContent()
		{
			$this->GetWebsite()->SetFramework("fullwidth");
			$this->GetWebsite()->MaintainScrollPosition();
			
 
 			$strAccountDataDir = GetDocumentRootDir();
 			$strAccountDataDir .= '../../';
 			if (GetStage() == 'dev')
 			{
 				$strAccountDataDir .= 'data-dev/';	
 			}
 			else
 			{
 				$strAccountDataDir .= 'data/';
 			}
 			
 			$strAccountDataDir .= 'mandates/demo/accounts/';
 
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
			

			
			$strHtml = '';
			
			
			$this->IncludeModule('aml/accountanalysis');
			
	

	
			if (ArrayCount($arrayAccounts) == 0)
			{
				$message = new CMessageElement($this->GetWebsite(),'info');
				$strHtml = $message->RenderMessage("No data available.");
			}
			else
			{
				
				$arrayNetworkGraph = AML_TransformAccountsToNetworkGraph($arrayAccounts);
				
				$strHtml .= '<h1>Network Analysis</h1>'."\n";
				$this->GetWebsite()->IncludeElement('amlnetworkgraph');
				$amlnetworkgraph = new CAMLNetworkGraphElement($this->GetWebsite(),'demo');
				$strHtml .= $amlnetworkgraph->RenderAMLNetworkGraph($arrayNetworkGraph);
				
				$nAccount = 0;
				foreach ($arrayAccounts as $arrayAccount)
				{
					$accountAnalysisModule = new CAccountAnalysisModule($this->GetWebsite(),'account'.$nAccount);
					$accountAnalysisModule->SetAccount($arrayAccount);
					$strHtml .= $accountAnalysisModule->RenderModule();
					$nAccount++;
				}	
				
			}
			
			
			
			

			
			
			
			
			
			return $strHtml;
			
			
		}
	}
	
