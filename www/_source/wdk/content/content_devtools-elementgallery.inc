<?php

	class CMyContent extends CContentDevtools
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->IncludeModule("devtools/elementgallery");
			$eg = new CElementGalleryModule($this->GetWebsite());
			$arrayItems["ELEMENTGALLERY"] = $eg->RenderModule();
		}
	}
	
