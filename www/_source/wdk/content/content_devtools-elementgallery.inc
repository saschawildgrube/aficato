<?php

	class CMyContent extends CContentDevtools
	{
		function OnRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->IncludeModule("devtools/elementgallery");
			$eg = new CElementGalleryModule($this->GetWebsite());
			$arrayItems["ELEMENTGALLERY"] = $eg->RenderModule();
		}
	}
	
