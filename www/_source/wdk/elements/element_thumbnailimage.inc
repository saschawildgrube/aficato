<?php

	class CThumbnailImageElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				"thumbnailimage",
				$strStyleID,
				false,
				true); 
		}
		
		function RenderThumbnailImage(
			$strThumbnailImageURL,
			$strImageURL,
			$nImageWidth,
			$nImageHeight,
			$strText)
		{
			$strOutput = $this->GetLayoutHTML(); 
			
			$arrayItems = array();
			
			$arrayItems["THUMBNAILIMAGEURL"] = $strThumbnailImageURL;
			$arrayItems["IMAGEURL"] = $strImageURL;
			$strImageSize = "";
			$nImageWidth = intval($nImageWidth);
			if ($nImageWidth > 0)
			{
				$strImageSize .= "width=$nImageWidth,";
			}
			$nImageHeight = intval($nImageHeight);
			if ($nImageHeight > 0)
			{
				$strImageSize .= "height=$nImageHeight,";
			}
			$arrayItems["IMAGESIZE"] = $strImageSize;
			$arrayItems["TEXT"] = $strText;
			
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strOutput);
			return $strOutput;			

		}
	}

