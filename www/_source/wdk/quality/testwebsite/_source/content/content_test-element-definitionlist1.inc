<?php

	class CMyContent extends CContent
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$this->IncludeElement("definitionlist");
			$definitionlist = new CDefinitionListElement($this->GetWebsite(),"default");
			$arrayDefList = array();
			$arrayDefList["Alpha"] = "This first letter in the Greek alphabet";
			$arrayDefList["Zulu"] = array("Represents the letter Z in the NATO alphabet","An African tribe");
			$arrayItems["ELEMENT_DEFINITIONLIST"] = $definitionlist->RenderDefinitionList($arrayDefList);
		}
	}
	
