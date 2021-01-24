<?php

	class CMyContent extends CContent
	{
		function OnRenderContent(&$arrayItems)
		{
			$this->IncludeElement("tabnavigation");
			$tabnavigationDef = new CTabNavigationDefinition();
			
			for ($nIndex = 1; $nIndex <= 3; $nIndex++)
			{
				$strText = "Tab $nIndex";
				$tabnavigationDef->AddTab(
					"$nIndex",
					$strText,
					"http://www.example.com/$nIndex");
			}
			$tabnavigation = new CTabNavigationElement($this->GetWebsite(),"default");
			$arrayItems["ELEMENT_TABNAVIGATION1"] = $tabnavigation->RenderTabNavigation($tabnavigationDef,"1");
			$arrayItems["ELEMENT_TABNAVIGATION2"] = $tabnavigation->RenderTabNavigation($tabnavigationDef,"2");
			$arrayItems["ELEMENT_TABNAVIGATION3"] = $tabnavigation->RenderTabNavigation($tabnavigationDef,"5");


		}
	}
	
