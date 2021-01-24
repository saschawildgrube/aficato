<?php



	class CXMLElement extends CDataContainer
	{
		public $m_nErrorCode;
		public $m_strErrorCode;

		
		function __construct()
		{
			$this->Init();	
		}
		
		function Init()
		{
			$this->m_nErrorCode = 0;
			$this->m_strErrorCode = "";

			$this->SetValue(array(),"children");	
			$this->SetValue(array(),"attributes");
			$this->SetData("");
			$this->SetName("");
		}
		
		function SetName($strName)
		{
			$this->SetValue($strName,"name");	
		}
		
		function GetName()
		{
			return $this->GetValue("name");	
		}
		
		function SetAttribute($strAttributeName, $strAttributeContent)
		{
			$this->SetValue($strAttributeContent,"attributes",$strAttributeName);
		}
		function GetAttribute($strAttributeName)
		{
			$this->GetValue("attributes",	$strAttributeName);
		}
		function GetAttributes()
		{
			return $this->GetValue("attributes");
		}
		
		function SetData($strData)
		{
			$this->SetValue($strData,"data");	
		}
		function GetData()
		{ 
			return $this->GetValue("data");	
		}		
		
		function AddChild(CXMLElement $xmlelement)
		{
			$arrayChildren = $this->GetValue("children");
			array_push($arrayChildren,$xmlelement->GetDataArray());
			$this->SetValue($arrayChildren,"children");
		}
		
		function AddChildData($strChildName,$strData)
		{
			$xmlelement = new CXMLElement();
			$xmlelement->SetName($strChildName);
			$xmlelement->SetData($strData);
			$this->AddChild($xmlelement);
		}
		
		function GetChildByIndex($nIndex)
		{
			$arrayChildren = $this->GetValue("children");
			return ArrayGetValue($arrayChildren,$nIndex);
		}
		
			
		function GetFirstChildByName($strName)
		{
			$arrayChildren = $this->GetValue("children");
			for ($nIndex = 0; $nIndex < ArrayCount($arrayChildren); $nIndex++)
			{
				$strChildName = ArrayGetValue($arrayChildren[$nIndex],"name");
				if ($strChildName == $strName)
				{
					$element = new CXMLElement();
					$element->SetDataArray($arrayChildren[$nIndex]);
					return $element;
				}
			}
			return false;
		}
		
		function GetChildren()
		{
			$arrayChildrenRaw = $this->GetValue("children");
			if (!is_array($arrayChildrenRaw))
			{
				return array();	
			}
			$arrayChildren = array();
			
			foreach ($arrayChildrenRaw as $arrayChildRaw)
			{
				$child = new CXMLElement();
				$child->SetDataArray($arrayChildRaw);
				$arrayChildren[] = $child;
			}
			return $arrayChildren;	
		}
		
		function GetFirstChildDataByName($strName)
		{
			$xmlChild = $this->GetFirstChildByName($strName);
			if ($xmlChild == false)
			{
				return "";
			}
			return $xmlChild->GetData();
		}
		
		function SetRecursiveArray($arrayData)
		{
			$this->SetData("");
			$this->SetValue(array(),"children");
			if (!is_array($arrayData))
			{
				return true;
			}
		
			$nCount = ArrayCount($arrayData);
			if ($nCount > 0)
			{
				$this->SetAttribute("COUNT",ArrayCount($arrayData));
			}
		
			foreach ($arrayData as $key => $value)
			{
				$elementChild = new CXMLElement();
				if (is_int($key)) 
				{ 
					$strKey = "ELEMENT"; 
					$elementChild->SetAttribute("INDEX",$key); 
				} 
				else 
				{
					$strKey = $key;         
				} 
				$elementChild->SetName($strKey);
				if (is_array($value))
				{
					$elementChild->SetRecursiveArray($value);	
				}
				else
				{
					$elementChild->SetData($value);
				}
				$this->AddChild($elementChild);
				
			} 
			return true;			
		}
		
		function GetRecursiveArray()
		{
			$arrayData = array();
	
			$arrayChildren = $this->GetChildren();
		
			foreach ($arrayChildren as $elementChild)
			{
				$strName = $elementChild->GetName();
				$arrayChildren1 = $elementChild->GetChildren();
				if (ArrayCount($arrayChildren1) > 0)
				{
					$data = $elementChild->GetRecursiveArray();
				}
				else
				{
					$data = $elementChild->GetData();	
				}
	
				$key = $strName;
				if ($strName == "ELEMENT")
				{
					$arrayAttributes = $elementChild->GetAttributes();
					$strIndex = ArrayGetValue($arrayAttributes,"INDEX");
					if ($strIndex != "" || $strIndex == 0)
					{
						$key = intval($strIndex);
					}
				}
				
				$arrayData[$key] = $data;
				
			}
			return $arrayData;
		}
		
		function Parse($strXML)
		{
			//$strXML = ReplaceString($strXML,'<br />',"\n");
			$this->Init();
			$arrayData = array();
			$arrayIndex = array();
			$xml_parser = xml_parser_create(); 
			$bXMLParseResult = xml_parse_into_struct(
				$xml_parser,
				$strXML,
				$arrayData,
				$arrayIndex);
			$this->m_nErrorCode = xml_get_error_code($xml_parser);
			$this->m_strErrorCode = xml_error_string($this->m_nErrorCode);
			xml_parser_free($xml_parser);
			unset($arrayIndex);
				
			if (	$bXMLParseResult == 0
				&& $this->m_nErrorCode != 27 // XML_WAR_UNDECLARED_ENTITY
				) 
			{
				return false;
			}
			if (!is_array($arrayData))
			{
				return false;
			}
			$nCount = ArrayCount($arrayData);
			if ($nCount == 0)
			{
				return false;	
			}
			if ($nCount == 1)
			{
				$arrayDataItem = $arrayData[0];
				if (!is_array($arrayDataItem))
				{
					return false;	
				}
				if ($arrayDataItem["type"] != "complete")
				{
					return false;	
				}
				$this->SetName($arrayDataItem["tag"]);
				$this->SetData(ArrayGetValue($arrayDataItem,"value"));
				return true;
			}

			$arrayXmlElements = array();			
			$nCurrentLevel = 1;
			
			for ($nIndex = 0; $nIndex < $nCount; $nIndex++)
			{
				$arrayDataItem = $arrayData[$nIndex];
				$arrayData = ArrayRemoveKey($arrayData,$nIndex);
				
				$strType = $arrayDataItem["type"];
				$nLevel = $arrayDataItem["level"];
				$strTag = $arrayDataItem["tag"];
				
				switch ($strType)	
				{
				case "open":
					$arrayXmlElements[$nCurrentLevel] = new CXMLElement();
					$arrayXmlElements[$nCurrentLevel]->SetName($strTag);
					$arrayAttributes = ArrayGetValue($arrayDataItem,"attributes");
					if (is_array($arrayAttributes))
					{
						foreach ($arrayAttributes as $key => $value)
						{
							$arrayXmlElements[$nCurrentLevel]->SetAttribute($key,$value);
						}
					}
					$nCurrentLevel++;
					break;
					
				case "close":
					$nCurrentLevel--;
					if ($nCurrentLevel > 1)
					{
						$arrayXmlElements[$nCurrentLevel-1]->AddChild($arrayXmlElements[$nCurrentLevel]);
						$arrayXmlElements = ArrayRemoveKey($arrayXmlElements,$nCurrentLevel);
					}
					
					break;				
				
				case "complete":
					$xmlNew = new CXMLElement();
					$xmlNew->SetName($strTag);
					$arrayAttributes = ArrayGetValue($arrayDataItem,"attributes");
					if (is_array($arrayAttributes))
					{
						foreach ($arrayAttributes as $key => $value)
						{
							$xmlNew->SetAttribute($key,$value);
						}
					}
					$strValue = ArrayGetValue($arrayDataItem,"value");
					if ($strValue!="")
					{
						$strValue = html_entity_decode($strValue,ENT_QUOTES,"UTF-8");
						//$strValue = html_entity_decode($strValue);
						$xmlNew->SetData($strValue);
					}
								
					$arrayXmlElements[$nCurrentLevel-1]->AddChild($xmlNew);
					break;
					
				default:	
					break;
				}
				
			}
			
			unset($arrayData);
			
			if (!ArrayKeyExists($arrayXmlElements,1))
			{
				return false;
			}
						
			$this->SetDataArray($arrayXmlElements[1]->GetDataArray());
	
			return true;
		}		
		
		
		
		
		
		
		function Render($bRawData = false,$strEOL = "\n", $nLevel = 0)
		{
			$strOutput = "";

			$strName = $this->GetName();
			if ($strName == "")
			{
				if ($strName == 0)
				{
					$strName = "0";	
				}
				else
				{
					return "";
				}
			}
			$strAttributes = "";
			
			$arrayAttributes = $this->GetAttributes();
			if (is_array($arrayAttributes))
			{
				
				for (reset($arrayAttributes); $strKey = key($arrayAttributes); next($arrayAttributes))
				{
					if ($strKey != "")
					{
						$strAttributes .= " ".$strKey."=\"". HtmlEncode(pos($arrayAttributes)) . "\"";
					}
				}
			}
				
			$strIndent = str_repeat("\t",$nLevel);
		
			$bHasData = false;
			$bHasChildren = false;
			
			$strData = $this->GetData();
			if ($strData != "")
			{
				$bHasData = true;
			}
			$arrayChildren = $this->GetChildren();
			if (is_array($arrayChildren))
			{
				if (ArrayCount($arrayChildren) > 0)
				{
					$bHasChildren = true;
				}
			}
			$strOutput .= $strIndent. "<".$strName.$strAttributes;
			if ($bHasData || $bHasChildren)
			{
				$strOutput .= ">";
				if ($bHasData)
				{
					$strData = strval($strData);
					if ($bRawData  == false)
					{
						$strData = HtmlEncode($strData);
						$strData = $this->MakeCDATA($strData);
					}
					$strOutput .= $strData;
				}
				if ($bHasChildren)
				{
					foreach ($arrayChildren as $key => $value)
					{
						$strOutput .= $strEOL;
						$childElement = $value;
						$strOutput .= $childElement->Render($bRawData,$strEOL,$nLevel+1);
					}
					$strOutput .= $strEOL;
					$strOutput .= $strIndent;
				}
				$strOutput .= "</".$strName.">";	
			}
			else
			{
				$strOutput .= "/>";
			}
			if ($nLevel == 0)
			{
				$strOutput .= $strEOL;
			}
			return $strOutput;
		}
		
		
		private
		function MakeCDATA($strData)
		{
			$strData = ReplaceString(strval($strData),']]>',']]]]><![CDATA[>');
			//$strData = ReplaceString($strData,"\n",'&#xA;&#xD;');
			//$strData = ReplaceString($strData,"\n",'<br />');
			
			$strData =  '<![CDATA['.$strData.']]>';
			return $strData;
		}
	}



	class CXMLDocument extends CXMLElement
	{
		function RenderDocument()
		{
			$strOutput = "<?xml version='1.0' encoding='utf-8' ?>\n";
			$strOutput .= parent::Render(); 
			return $strOutput;
		}	
		
	}




	abstract
	class CXMLProcessor
	{
		function Process($strXML,$arrayParams,$strIndent = "\t")
		{
			$strOutputXML = "";
			$arrayData = array();
			$arrayIndex = array();
			
			$xml_parser = xml_parser_create("UTF-8");
			xml_parser_set_option($xml_parser, XML_OPTION_CASE_FOLDING, 0);
			$bXMLParseResult = xml_parse_into_struct(
				$xml_parser,
				$strXML,
				$arrayData,
				$arrayIndex);

				
			if ($bXMLParseResult == 0)
			{
				//print("bXMLParseResult = $bXMLParseResult\nError: ".xml_error_string(xml_get_error_code($xml_parser))." at line ".xml_get_current_line_number($xml_parser). ", column ".xml_get_current_column_number($xml_parser)."\n");  
				xml_parser_free($xml_parser);
				return false;
			}
			xml_parser_free($xml_parser);
			if (!is_array($arrayData))
			{
				return false;
			}
			
			$nCount = ArrayCount($arrayData);
			if ($nCount == 0)
			{
				return false;	
			}
			
			$strEOL = StringGetEndOfLineToken($strXML);

			$nCurrentLevel = 0;
			
			for ($nIndex = 0; $nIndex < $nCount; $nIndex++)
			{
				$arrayDataItem = $arrayData[$nIndex];
				
				$strType = $arrayDataItem["type"];
				$nLevel = $arrayDataItem["level"];
				$strTag = $arrayDataItem["tag"];
				
				switch ($strType)	
				{
				case "open":
					$strAttributes = "";
					$arrayAttributes = ArrayGetValue($arrayDataItem,"attributes");
					if (is_array($arrayAttributes))
					{
						foreach ($arrayAttributes as $key => $value)
						{
							$strAttributes .= " $key=\"$value\"";
						}
					}
					$strOutputXML .= 
						str_repeat($strIndent,$nCurrentLevel)
						."<".$strTag.$strAttributes.">"
						.$strEOL;
					$nCurrentLevel++;
					break;
					
				case "close":
					$nCurrentLevel--;
					$strOutputXML .= 
						str_repeat($strIndent,$nCurrentLevel)
						."</".$strTag.">"
						.$strEOL;
					break;				
				
				case "complete":
					$strAttributes = "";
					$arrayAttributes = ArrayGetValue($arrayDataItem,"attributes");
					if (is_array($arrayAttributes))
					{
						foreach ($arrayAttributes as $key => $value)
						{
							$strAttributes .= " $key=\"$value\"";
						}
					}
					$strValue = trim(ArrayGetValue($arrayDataItem,"value"));
					$strValue = $this->OnData($strTag,$strValue,$arrayParams);
					if ($strValue == "")
					{
						$strOutputXML .= 
							str_repeat($strIndent,$nCurrentLevel)
							."<".$strTag.$strAttributes."/>"
							.$strEOL;
					}
					else
					{
						$strOutputXML .= 
							str_repeat($strIndent,$nCurrentLevel)
							."<".$strTag.$strAttributes.">"
							.$strEOL;
						$strOutputXML .=
							str_repeat($strIndent,$nCurrentLevel+1)
							.$strValue
							.$strEOL;
						$strOutputXML .= 
							str_repeat($strIndent,$nCurrentLevel)
							."</".$strTag.">"
							.$strEOL;							

						
						/*
						$strOutputXML .= 
							str_repeat($strIndent,$nCurrentLevel)
							."<".$strTag.$strAttributes.">"
							.$strValue
							."</".$strTag.">"
							.$strEOL;														
							*/
					}
					break;
					
				default:	
					break;
				}
				
			}
			
			return $strOutputXML;			

		}
				
		function OnData($strElementName,$strData,$arrayParams)
		{
			return $strData;
		}
		
		function OnAttributes($strElementName,$arrayAttributes)
		{
			return $arrayAttributes;
		}		


	}

	class CAddEmbracingTagsXMLProcessor extends CXMLProcessor
	{
		function AddEmbracingTags(
			$strXML,
			$strToken,
			$strTagStart,
			$strTagEnd,
			$bIgnoreCase,
			$arrayTagWhitelist,
			$arrayTagBlacklist)
		{
			$arrayParams = array();
			$arrayParams["token"] = $strToken;
			$arrayParams["tagstart"] = $strTagStart;
			$arrayParams["tagend"] = $strTagEnd;
			$arrayParams["ignorecase"] = $bIgnoreCase;
			$arrayParams["whitelist"] = $arrayTagWhitelist;
			$arrayParams["blacklist"] = $arrayTagBlacklist;
			return $this->Process($strXML,$arrayParams,"");
		}
		
		function OnData($strElementName,$strData,$arrayParams)
		{
			$strData = StringXMLEntities($strData);
			$arrayWhitelist = $arrayParams["whitelist"];
			if (is_array($arrayWhitelist))
			{
				if (ArrayCount($arrayWhitelist)>0)
				{
					if (!ArrayValueExists($arrayWhitelist,$strElementName))
					{
						return $strData;	
					}	
				}
			}

			$arrayBlacklist = $arrayParams["blacklist"];
			if (is_array($arrayBlacklist))
			{
				if (ArrayCount($arrayBlacklist)>0)
				{
					if (ArrayValueExists($arrayBlacklist,$strElementName))
					{
						return $strData;	
					}	
				}
			}

			
			$strTagStart = $arrayParams["tagstart"];
			$strTagEnd = $arrayParams["tagend"];
			$strToken = $arrayParams["token"];
			$bIgnoreCase = $arrayParams["ignorecase"];	

	
			if ($bIgnoreCase == true)
			{
				return StringAddEmbracingTagsIgnoreCase(
					$strData,$strToken,$strTagStart,$strTagEnd);	
			}
			else
			{
				return StringAddEmbracingTags(
					$strData,$strToken,$strTagStart,$strTagEnd);	
			}
		}
	}


	function StringXMLAddEmbracingTags(
		$strXML,
		$strToken,
		$strTagStart,
		$strTagEnd,
		$bIgnoreCase,
		$arrayTagWhitelist,
		$arrayTagBlacklist)
	{
		if ($strToken == "")
		{
			return $strXML;
		}
		do
		{
			$strUnique = uniqid();
		} while (
					(FindString($strUnique,$strToken) != -1)
			&&	(FindString($strUnique,$strXML) != -1));
		
		$strXML_tweak = "<ROOT_".$strUnique.">".$strXML."</ROOT_".$strUnique.">";
		$strXML_tweak = ReplaceString($strXML_tweak,"&amp;","AMP_".$strUnique);
		$strToken = ReplaceString($strToken,"&amp;","AMP_".$strUnique);
		
		$xmlProcessor = new CAddEmbracingTagsXMLProcessor();
		$strOutput = $xmlProcessor->AddEmbracingTags(
			$strXML_tweak,
			$strToken,
			$strTagStart,
			$strTagEnd,
			$bIgnoreCase,
			$arrayTagWhitelist,
			$arrayTagBlacklist);
		if ($strOutput == false)
		{
			return false;
		}
		$strOutput = ReplaceString($strOutput,"<ROOT_".$strUnique.">","");
		$strOutput = ReplaceString($strOutput,"</ROOT_".$strUnique.">","");
		$strOutput = ReplaceString($strOutput,"AMP_".$strUnique,"&amp;");
		return $strOutput;	
	}



	function GetAttributeFromXMLTag($strTag, $strAttrID)
	{
		$strValue = StringGetFromEmbracingTags(
			$strTag,
			' '.$strAttrID.'="',
			'"');
		if ($strValue != '')
		{
			return $strValue;
		}
		$strValue = StringGetFromEmbracingTags(
			$strTag,
			"\t".$strAttrID.'="',
			'"');
		if ($strValue != '')
		{
			return $strValue;
		}
		$strValue = StringGetFromEmbracingTags(
			$strTag,
			"\n".$strAttrID.'="',
			'"');
		return $strValue;
	}


	function StringXMLEntities($strValue)
	{ 
		$strValue = StringEncodeUTF8($strValue); 

    // Only do the slow convert if there are 8-bit characters
		// avoid using 0xA0 (\240) in ranges. RH73 does not like that

		if (! preg_match("/[\200-\237]/",$strValue) && !preg_match("/[\241-\377]/",$strValue))
		{
			return $strValue; 
		}

		// decode three byte unicode characters 
		$strValue = preg_replace_callback(
			"/([\340-\357])([\200-\277])([\200-\277])/",
			function($arrayMatch)
			{
				return '&#'.((ord($arrayMatch[1])-224)*4096 + (ord($arrayMatch[2])-128)*64 + (ord($arrayMatch[3])-128)).';';
			},
			$strValue); 

		// decode two byte unicode characters 
		$strValue = preg_replace_callback(
			"/([\300-\337])([\200-\277])/",
			function ($arrayMatch)
			{
				return '&#'.((ord($arrayMatch[1])-192)*64+(ord($arrayMatch[2])-128)).';';
			}, 
			$strValue); 
						
		return $strValue; 
	} 
	


