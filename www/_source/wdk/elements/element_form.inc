<?php

	class CFormDefinition
	{
		private $m_arrayData;
		
		function __construct()
		{
			$this->m_arrayData = array();	
			$this->m_arrayData["controls"] = array();
		}
		
		function GetData()
		{
			return $this->m_arrayData;	
		}
		
		function SetURL($strURL,$arrayParams)
		{
			$this->m_arrayData["url"] = $strURL;
			$this->m_arrayData["params"] = $arrayParams;
		}
		
		function SetHiddenValue($strParam,$strValue)
		{
			$this->m_arrayData["params"][$strParam] = $strValue;
		}
		
		function AddControlText($strID,$strName,$strLabel,$strDefaultValue,$nSize,$nMaxLength,$bReadOnly = false)
		{
			$arrayControl = array();
			$arrayControl["id"] = $strID;
			$arrayControl["name"] = $strName;
			$arrayControl["type"] = "text";
			$arrayControl["label"] = $strLabel;
			$arrayControl["defaultvalue"] = $strDefaultValue;
			$arrayControl["width"] = $nSize;
			$arrayControl["maxlength"] = $nMaxLength;
			$arrayControl["readonly"] = $bReadOnly;
			array_push($this->m_arrayData["controls"],$arrayControl);
		}
		
		function AddControlTextArea($strID,$strName,$strLabel,$strDefaultValue,$nCols,$nRows,$bReadOnly = false)
		{
			$arrayControl = array();
			$arrayControl["id"] = $strID;
			$arrayControl["name"] = $strName;
			$arrayControl["type"] = "textarea";
			$arrayControl["label"] = $strLabel;
			$arrayControl["defaultvalue"] = $strDefaultValue;
			$arrayControl["width"] = $nCols;
			$arrayControl["rows"] = $nRows;
			$arrayControl["readonly"] = $bReadOnly;			
			array_push($this->m_arrayData["controls"],$arrayControl);
		}

		function AddControlPassword($strID,$strName,$strLabel,$strDefaultValue,$nSize,$nMaxLength,$bReadOnly = false)
		{
			$arrayControl = array();
			$arrayControl["id"] = $strID;
			$arrayControl["name"] = $strName;
			$arrayControl["type"] = "password";
			$arrayControl["label"] = $strLabel;
			$arrayControl["defaultvalue"] = $strDefaultValue;
			$arrayControl["width"] = $nSize;
			$arrayControl["maxlength"] = $nMaxLength;
			$arrayControl["readonly"] = $bReadOnly;
			array_push($this->m_arrayData["controls"],$arrayControl);
		}

		function AddControlCheckbox($strID,$strName,$strLabel,$bDefaultValue,$bReadOnly = false)
		{
			$arrayControl = array();
			$arrayControl["id"] = $strID;
			$arrayControl["name"] = $strName;
			$arrayControl["type"] = "checkbox";
			$arrayControl["label"] = $strLabel;
			$arrayControl["defaultvalue"] = $bDefaultValue;
			$arrayControl["readonly"] = $bReadOnly;
			array_push($this->m_arrayData["controls"],$arrayControl);
		}


		function AddControlSelect($strID,$strName,$strLabel,$arrayOptions,$strDefaultValue,$bReadOnly = false)
		{
			$arrayControl = array();
			$arrayControl["id"] = $strID;
			$arrayControl["name"] = $strName;
			$arrayControl["type"] = "select";
			$arrayControl["label"] = $strLabel;
			$arrayControl["defaultvalue"] = $strDefaultValue;
			$arrayControl["options"] = $arrayOptions;
			$arrayControl["readonly"] = $bReadOnly;
			array_push($this->m_arrayData["controls"],$arrayControl);
		}

		function AddControlRadio($strID,$strName,$strLabel,$arrayOptions,$strDefaultValue,$bReadOnly = false)
		{
			$arrayControl = array();
			$arrayControl["id"] = $strID;
			$arrayControl["name"] = $strName;
			$arrayControl["type"] = "radio";
			$arrayControl["label"] = $strLabel;
			$arrayControl["defaultvalue"] = $strDefaultValue;
			$arrayControl["options"] = $arrayOptions;
			$arrayControl["readonly"] = $bReadOnly;
			array_push($this->m_arrayData["controls"],$arrayControl);
		}

		function AddControlDate($strID,$strName,$strLabel,$strDefaultValue,$bReadOnly = false)
		{
			$arrayControl = array();
			$arrayControl["id"] = $strID;
			$arrayControl["name"] = $strName;
			$arrayControl["type"] = "date";
			$arrayControl["label"] = $strLabel;
			$arrayControl["defaultvalue"] = $strDefaultValue;
			//$arrayControl["options"] = $arrayOptions;
			$arrayControl["readonly"] = $bReadOnly;
			array_push($this->m_arrayData["controls"],$arrayControl);
		}

		function AddControlDateTime($strID,$strName,$strLabel,$strDefaultValue,$bReadOnly = false)
		{
			$arrayControl = array();
			$arrayControl["id"] = $strID;
			$arrayControl["name"] = $strName;
			$arrayControl["type"] = "datetime";
			$arrayControl["label"] = $strLabel;
			$arrayControl["defaultvalue"] = $strDefaultValue;
			//$arrayControl["options"] = $arrayOptions;
			$arrayControl["readonly"] = $bReadOnly;
			array_push($this->m_arrayData["controls"],$arrayControl);
		}

		function AddControlFile($strID,$strName,$strLabel,$bReadOnly = false)
		{
			$arrayControl = array();
			$arrayControl["id"] = $strID;
			$arrayControl["name"] = $strName;
			$arrayControl["type"] = "file";
			$arrayControl["label"] = $strLabel;
			$arrayControl["readonly"] = $bReadOnly;
			array_push($this->m_arrayData["controls"],$arrayControl);
		}

		/*
		In most cases, there is only one submit button, so no need for
		specific ID or name settings. That is why the parameter order is
		different here.
		*/
		function AddControlButtonSubmit($strLabel,$strID = "SUBMIT", $strValue="")
		{
			$arrayControl = array();
			$arrayControl["id"] = $strID;
			$arrayControl["name"] = "submit";
			$arrayControl["type"] = "submit";
			$arrayControl["label"] = $strLabel;
			$arrayControl["defaultvalue"] = $strValue;
			array_push($this->m_arrayData["controls"],$arrayControl);
		}

		function AddControlButtonSubmitDownload($strLabel,$strID = "SUBMIT", $strValue="")
		{
			$strDownloadToken = MakeNumericToken(6);
			$this->SetHiddenValue("downloadtoken",$strDownloadToken);
			
			$arrayControl = array();
			$arrayControl["id"] = $strID;
			$arrayControl["name"] = "submit";
			$arrayControl["type"] = "submitdownload";
			$arrayControl["label"] = $strLabel;
			$arrayControl["defaultvalue"] = $strValue;
			$arrayControl["downloadtoken"] = $strDownloadToken;
			array_push($this->m_arrayData["controls"],$arrayControl);
		}


		function AddControlButtonReset($strLabel)
		{
			$arrayControl = array();
			$arrayControl["id"] = "RESET";
			$arrayControl["name"] = "reset";
			$arrayControl["type"] = "reset";
			$arrayControl["label"] = $strLabel;
			array_push($this->m_arrayData["controls"],$arrayControl);
		}
		
		function AddControlStatic($strID,$strPayload)
		{
			$arrayControl = array();
			$arrayControl["id"] = $strID;
			$arrayControl["name"] = "static";
			$arrayControl["type"] = "static";
			$arrayControl["payload"] = $strPayload;
			array_push($this->m_arrayData["controls"],$arrayControl);
		}
	
		function AddControlSeparator()
		{
			$arrayControl = array();
			$arrayControl["id"] = "SEPARATOR";
			$arrayControl["name"] = "separator";
			$arrayControl["type"] = "separator";
			array_push($this->m_arrayData["controls"],$arrayControl);
		}
	
		function AddControlReadOnly($strID,$strLabel,$strPayload)
		{
			$arrayControl = array();
			$arrayControl["id"] = $strID;
			$arrayControl["name"] = "readonly";
			$arrayControl["type"] = "readonly";
			$arrayControl["label"] = $strLabel;
			$arrayControl["payload"] = $strPayload;
			array_push($this->m_arrayData["controls"],$arrayControl);
		}
		
		function IsValid(&$strErrorText = "")
		{
			if (ArrayGetValue($this->m_arrayData,"url") == "")
			{
				$strErrorText = "URL is not set.";
				return false;	
			}
			$arrayParams = ArrayGetValue($this->m_arrayData,"params");
			if (!is_array($arrayParams))
			{
				$strErrorText = "\"params\" is not an array.";
				return false;	
			}
			$arrayControls = ArrayGetValue($this->m_arrayData,"controls");
			if (!is_array($arrayControls))
			{
				$strErrorText = "\"controls\" is not an array.";
				return false;	
			}
			foreach ($arrayControls as $control)
			{
				$strType = ArrayGetValue($control,"type");
				if ($strType == "")
				{
					$strErrorText = "\"type\" is not set.";
					return false;	
				}				
				$strID = ArrayGetValue($control,"id");
				if ($strID == "")
				{
					$strErrorText = "\"id\" is not set.";
					return false;	
				}
				if ($strID != StringUpperCase($strID))
				{
					$strErrorText = "id \"$strID\" must be uppercase.";
					return false;	
				}
				if (!ctype_alpha(StringSection($strID,0,1)))
				{
					$strErrorText = "id \"$strID\" must start with a character (it must not be a number).";
					return false;	
				}
				$strName = ArrayGetValue($control,"name");
				if ($strName == "")
				{
					$strErrorText = "\"name\" is not set.";
					return false;	
				}				
			}
			return true;
		}
		
	}
	
	abstract
	class CFormElementHelper extends CElement
	{
		private $m_strFormInstanceToken;
		function __construct(
			CWebSite $website,
			$strElementID,
			$strStyleID,
			$bLayoutDef,
			$bLayoutHTML)
		{
			parent::__construct(
				$website,
				$strElementID,
				$strStyleID,
				$bLayoutDef,
				$bLayoutHTML);
		}
		
		protected
		function InitFormInstanceToken()
		{
			$this->m_strFormInstanceToken = uniqid();
		}
		
		protected
		function GetFormInstanceToken()
		{
			return $this->m_strFormInstanceToken;
		}
		
		protected
		function GetValueForControl($control,&$arrayValues)
		{
			$strName = ArrayGetValue($control,"name");
			if (ArrayKeyExists($arrayValues,$strName) == true)
			{
				return ArrayGetValue($arrayValues,$strName);
			}
			return ArrayGetValue($control,"defaultvalue");
		}
		
		protected
		function RenderLabel($control,$arrayErrors)
		{
			$strID = ArrayGetValue($control,"id");
			$strName = ArrayGetValue($control,"name");
			$strType = ArrayGetValue($control,"type");
			$strLabelText = "";
			$bLabelFirst = true;
			if ($strType == "checkbox")
			{
				$bLabelFirst = GetBoolValue($this->GetLayoutDef("LAYOUT_CONTROL_CHECKBOX_LABELFIRST"));
			}
			if ($bLabelFirst == true)
			{
				$strLabelText = ArrayGetValue($control,"label");
			}
			$bMandatory = (ArrayGetValue($control,"mandatory") != "")?(true):(false);
			if (ArrayKeyExists($arrayErrors,$strID) == true)
			{
				$bError = true;	
			}
			else
			{
				$bError = false;	
			}
			
			
			$strLayoutPostfix = "";
			$strErrorText = "";
			if ($bMandatory)
			{
				$strLayoutPostfix .= "_MANDATORY";
			}
			if ($bError)
			{
				$strLayoutPostfix .= "_ERROR";
				if ($arrayErrors[$strID] != "")
				{
					$strLayoutPostfix .= "WITHTEXT";
					$strErrorText = $arrayErrors[$strID];
				}
			}
			$strLabel = $this->GetLayoutDef("LAYOUT_LABEL".$strLayoutPostfix."_".StringUpperCase($strType));
			if ($strLabel == "")
			{
				$strLabel = $this->GetLayoutDef("LAYOUT_LABEL".$strLayoutPostfix);
			}
			
			$strID = $this->GetControlID($control);
			$strClassLabel = $this->GetClassHTML_Label($control);			
			if ($strType == "readonly")
			{
				$strLabelTag = "<label $strClassLabel>$strLabelText</label>";
			}
			else
			{
				$strLabelTag = "<label for=\"". $strID . "\"$strClassLabel>$strLabelText</label>";
			}
				
			$strLabel = ReplaceString($strLabel,"{LABELTEXT}",$strLabelTag);
			if ($strErrorText != "")
			{
				$strLabel = ReplaceString($strLabel,"{ERRORTEXT}",$strErrorText);
			}
			return $strLabel . "\n";
		}
		
		protected
		function GetClassHTML_Form()
		{
			$strClass = $this->GetLayoutDef("LAYOUT_CLASS_FORM");
			if ($strClass == "")
			{
				return "";						
			}
			return " class=\"$strClass\"";
		}		
		
		protected
		function GetClassHTML_Control($control)
		{
			$strType = ArrayGetValue($control,"type");
			$strClass = $this->GetLayoutDef("LAYOUT_CLASS_CONTROL_".StringUpperCase($strType));
			if ($strClass == "")
			{
				if ($strType == "submitdownload")
				{
					$strClass = $this->GetLayoutDef("LAYOUT_CLASS_CONTROL_SUBMIT");
				}
				if ($strClass == "")
				{
					$strClass = $this->GetLayoutDef("LAYOUT_CLASS_CONTROL");
					if ($strClass == "")
					{
						return "";						
					}
				}					
			}
			if ($strClass == "!")
			{
				return "";	
			}
			return " class=\"$strClass\"";
		}

		protected
		function GetClassHTML_Label($control)
		{
			$strType = ArrayGetValue($control,"type");
			$strClass = $this->GetLayoutDef("LAYOUT_CLASS_LABEL_".StringUpperCase($strType));
			if ($strClass == "!")
			{
				return "";	
			}
			if ($strClass == "")
			{
				$strClass = $this->GetLayoutDef("LAYOUT_CLASS_LABEL");
				if ($strClass == "")
				{
					return "";						
				}
			}
			return " class=\"$strClass\"";
		}


		protected 
		function RenderControlText($control,&$arrayValues)
		{
			if (ArrayGetValue($control,"type") != "text")
			{
				$this->Error("Control is not of text type.");	
			}
			
			$strID = $this->GetControlID($control);
			$strName = ArrayGetValue($control,"name");
			$strValue = $this->GetValueForControl($control,$arrayValues);
			
			$nSize = intval(ArrayGetValue($control,"width"));
			$nMaxLength = intval(ArrayGetValue($control,"maxlength"));
			$strReadOnlyHTML = (ArrayGetValue($control,"readonly"))?(" readonly=\"readonly\""):("");
			$strClassHTML = $this->GetClassHTML_Control($control);

			$strInputHTML = "<input type=\"text\" id=\"$strID\" name=\"$strName\" value=\"$strValue\" size=\"$nSize\" maxlength=\"$nMaxLength\"$strReadOnlyHTML$strClassHTML/>\n";
			return $strInputHTML;
		
		}


		protected 
		function RenderControlPassword($control,&$arrayValues)
		{
			if (ArrayGetValue($control,"type") != "password")
			{
				$this->Error("Control is not of password type.");	
			}
			
			$strID = $this->GetControlID($control);
			$strName = ArrayGetValue($control,"name");
			$strValue = $this->GetValueForControl($control,$arrayValues);
			
			$nSize = intval(ArrayGetValue($control,"width"));
			$nMaxLength = intval(ArrayGetValue($control,"maxlength"));
			$strReadOnlyHTML = (ArrayGetValue($control,"readonly"))?(" readonly=\"readonly\""):("");
			$strClassHTML = $this->GetClassHTML_Control($control);
									
			$strInputHTML = "<input type=\"password\" id=\"$strID\" name=\"$strName\" value=\"$strValue\" size=\"$nSize\" maxlength=\"$nMaxLength\"$strReadOnlyHTML$strClassHTML/>\n";
			
			return $strInputHTML;
		
		}
		
		protected 
		function RenderControlTextArea($control,&$arrayValues)
		{
			if (ArrayGetValue($control,"type") != "textarea")
			{
				$this->Error("Control is not of textarea type.");	
			}
			
			$strID = $this->GetControlID($control);
			$strName = ArrayGetValue($control,"name");
			$strValue = $this->GetValueForControl($control,$arrayValues);
			$strReadOnlyHTML = (ArrayGetValue($control,"readonly"))?(" readonly=\"readonly\""):("");
			$strClassHTML = $this->GetClassHTML_Control($control);
			
			$nRows = intval(ArrayGetValue($control,"rows"));
			$nCols = intval(ArrayGetValue($control,"width"));
			$strInputHTML = "<textarea name=\"$strName\" id=\"$strID\" rows=\"$nRows\" cols=\"$nCols\"$strReadOnlyHTML$strClassHTML>$strValue</textarea>\n";
			return $strInputHTML;
		}

		protected 
		function RenderControlSelect($control,&$arrayValues)
		{
			if (ArrayGetValue($control,"type") != "select")
			{
				$this->Error("Control is not of select type.");	
			}
			
			$strID = $this->GetControlID($control);
			$strName = ArrayGetValue($control,"name");
			$strValue = $this->GetValueForControl($control,$arrayValues);
			$strReadOnlyHTML = (ArrayGetValue($control,"readonly"))?(" readonly=\"readonly\""):("");
			$strClassHTML = $this->GetClassHTML_Control($control);
		
			$arrayOptions = ArrayGetValue($control,"options");
			if (!is_array($arrayOptions))
			{
				$this->Error("Form select rendering failed. arrayOptions is not an array.");	
			}
			$strInputHTML = "<select id=\"$strID\" name=\"$strName\"$strReadOnlyHTML$strClassHTML>\n";
			
			foreach ($arrayOptions as $strOptionValue => $strOptionLabel)
			{
				$strSelectedHTML = "";
				if (strval($strOptionValue) == strval($strValue))
				{
					$strSelectedHTML = " selected=\"selected\"";
				}
				$strInputHTML .= "<option value=\"$strOptionValue\"$strSelectedHTML>$strOptionLabel</option>\n";
			}
			$strInputHTML .= "</select>\n";
			
			return $strInputHTML;
		}
		
		
		protected 
		function RenderControlRadio($control,&$arrayValues)
		{
			if (ArrayGetValue($control,"type") != "radio")
			{
				$this->Error("Control is not of radio type.");	
			}
			$arrayOptions = ArrayGetValue($control,"options");
			if (!is_array($arrayOptions))
			{
				$this->Error("Form radio rendering failed. arrayOptions is not an array.");	
			}
			$nCount = ArrayCount($arrayOptions);
			if ($nCount == 0)
			{
				$this->Error("Form radio rendering failed. arrayOptions is empty.");	
			}
			$strID = $this->GetControlID($control);
			$strName = ArrayGetValue($control,"name");
			$strValue = $this->GetValueForControl($control,$arrayValues);
			$strReadOnlyHTML = (ArrayGetValue($control,"readonly"))?(" readonly=\"readonly\""):("");
			$strClassHTML = $this->GetClassHTML_Control($control);			
			
			$strInputHTML = $this->GetLayoutDef("LAYOUT_CONTROL_RADIO_BEGIN");
			$nCurrent = 1;
			foreach ($arrayOptions as $option_value => $option_label)
			{
				$strIDHTML = "";
				if ($nCurrent == 1)
				{
					$strIDCurrent = "$strID";	
					$strIDHTML = "id=\"$strID\"";
				}
				else
				{
					$strIDCurrent = "$strID-$nCurrent";	
					$strIDHTML = "id=\"$strIDCurrent\"";
				}
				
				$strCheckedHTML = "";
				if ($option_value == $strValue)
				{
					$strCheckedHTML = " checked=\"checked\"";
				}
				$strClassLabel = $this->GetClassHTML_Label($control);    
				$strInputHTML .= "<input type=\"radio\" $strIDHTML name=\"$strName\" value=\"$option_value\"$strCheckedHTML$strReadOnlyHTML$strClassHTML/>$option_label\n";   // <label for=\"$strIDCurrent\">   </label>
				if ($nCurrent < $nCount)
				{
					$strInputHTML .= $this->GetLayoutDef("LAYOUT_CONTROL_RADIO_SEPARATOR");
				}
				$nCurrent++;
			}
			$strInputHTML .= $this->GetLayoutDef("LAYOUT_CONTROL_RADIO_END");	
			
			return $strInputHTML;
		}
		
		protected 
		function RenderControlCheckbox($control,&$arrayValues)
		{
			if (ArrayGetValue($control,"type") != "checkbox")
			{
				$this->Error("Control is not of checkbox type.");	
			}			
		
			$strReadOnlyHTML = (ArrayGetValue($control,"readonly"))?(" readonly=\"readonly\""):("");
			$strClassHTML = $this->GetClassHTML_Control($control);			
		
			$strLabelHTML = "";
			$strLabel = ArrayGetValue($control,"label");
			$strName = ArrayGetValue($control,"name");
			$strID = $this->GetControlID($control);
			
			$strCheckedHTML = "";
			$bValue = intval($this->GetValueForControl($control,$arrayValues));
			if ($bValue == 1)
			{
				$strCheckedHTML = " checked=\"checked\"";
			}
			else
			{
				$this->Trace("checkbox is off");	
			}
			
			$bLabelFirst = GetBoolValue($this->GetLayoutDef("LAYOUT_CONTROL_CHECKBOX_LABELFIRST"));
			if ($bLabelFirst == false)
			{
				$strLabelHTML = "<label for=\"$strID\">$strLabel</label>";
			}
			else
			{
				$strLabelHTML = "";
			}
			
			$strInputHTML = "<input type=\"checkbox\" id=\"$strID\" name=\"$strName\" value=\"1\"$strCheckedHTML$strReadOnlyHTML$strClassHTML/>$strLabelHTML\n";
			$strHiddenHTML = "<input type=\"hidden\" name=\"".$strName."\" value=\"0\"/>\n";
			$strInputHTML = $strHiddenHTML . $strInputHTML;
						
			/*
			
			HACK!
			
			A checkbox is defined as a key value pair which is only sent
			by browsers if the checkbox is checked.
			Shit by design: There is no alternative value sent for unchecked checkboxes.
			That is stupid, but this is how HTML is defined.
			
			This implies, that one cannot explicitly check the value of an unchecked checkbox.
			One can only check if the value is present at all.
			
			This means that if a checkbox is used to modify a session stored value, this
			change must be explicitly done, and cannot be made using generic methods.
			Because the parameter is not present, no modification of a session stored
			value is triggered and the session stored value remains unchanged.
			
			This conflicts with the "var" logic of the WDK.
			
			Solution:
			All components in a form can override each other
			
			*/		
			
			return $strInputHTML;
			
		}
		
		protected 
		function RenderControlDate($control,&$arrayValues)
		{
			if (ArrayGetValue($control,"type") != "date")
			{
				$this->Error("Control is not of date type.");	
			}					
			
			$strID = $this->GetControlID($control);
			$strName = ArrayGetValue($control,"name");
			$strValue = $this->GetValueForControl($control,$arrayValues);
			$strReadOnlyHTML = (ArrayGetValue($control,"readonly"))?(" readonly=\"readonly\""):("");
			$strClassHTML = $this->GetClassHTML_Control($control);			


			$nSize = 10;
			$nMaxLength = 10;
												
			$strInputHTML = "<input type=\"text\" id=\"$strID\" name=\"$strName\" value=\"$strValue\" size=\"$nSize\" maxlength=\"$nMaxLength\"$strReadOnlyHTML$strClassHTML/>\n";
			
			return $strInputHTML;
		}		

		protected 
		function RenderControlDateTime($control,&$arrayValues)
		{
			if (ArrayGetValue($control,"type") != "datetime")
			{
				$this->Error("Control is not of datetime type.");	
			}					
			
			$strID = $this->GetControlID($control);
			$strName = ArrayGetValue($control,"name");
			$strValue = $this->GetValueForControl($control,$arrayValues);
			$strReadOnlyHTML = (ArrayGetValue($control,"readonly"))?(" readonly=\"readonly\""):("");
			$strClassHTML = $this->GetClassHTML_Control($control);			

			$nDateTimeLength = StringLength(
				$this->GetWebsite()->RenderDateTime(
					GetTimeNow()));

			$nSize = $nDateTimeLength;
			$nMaxLength = $nDateTimeLength;
												
			$strInputHTML = "<input type=\"text\" id=\"$strID\" name=\"$strName\" value=\"$strValue\" size=\"$nSize\" maxlength=\"$nMaxLength\"$strReadOnlyHTML$strClassHTML/>\n";
			
			return $strInputHTML;
		}	

		protected 
		function RenderControlFile($control)
		{
			if (ArrayGetValue($control,"type") != "file")
			{
				$this->Error("Control is not of file type.");	
			}					
			
			$strID = $this->GetControlID($control);
			$strName = ArrayGetValue($control,"name");
			$strReadOnlyHTML = (ArrayGetValue($control,"readonly"))?(" readonly=\"readonly\""):("");
			$strClassHTML = $this->GetClassHTML_Control($control);			


			$nSize = ArrayGetValue($control,"size");
			
												
			//$strInputHTML = "<input type=\"file\" id=\"$strID\" name=\"$strName\" size=\"$nSize\" $strReadOnlyHTML/>\n";
			$strInputHTML = "<input type=\"file\" id=\"$strID\" name=\"$strName\" $strReadOnlyHTML$strClassHTML/>\n";
			
			return $strInputHTML;
		}
		
		protected 
		function RenderControlButtonSubmit($control)
		{
			$strType = ArrayGetValue($control,"type");
			if ($strType != "submit" && $strType != "submitdownload")
			{
				$this->Error("Control is not of type submit or submitdownload.");	
			}					

			$strLabelHTML = "";
			$strLabel = ArrayGetValue($control,'label');
			$strID = $this->GetControlID($control);
			$strName = ArrayGetValue($control,"name");
			$strValue = ArrayGetValue($control,"defaultvalue");
			
			$strButtonInnerArea = ReplaceString($this->GetLayoutDef("LAYOUT_CONTROL_BUTTON_INNERAREA"),"{BUTTONLABEL}",$strLabel);
			$strButtonInnerArea = "<!--\"$strValue\"-->".$strButtonInnerArea;
			$strClassHTML = $this->GetClassHTML_Control($control);			

			$strDataAttributes = "";
			
			if ($strType == "submitdownload")
			{
				$strDownloadToken = ArrayGetValue($control,'downloadtoken');
				if ($strDownloadToken != '')
				{
					$strDataAttributes .= ' data-downloadtoken="'.$strDownloadToken.'"';
				}
			}
			

			$strInputHTML = "<button type=\"submit\" id=\"$strID\" name=\"$strName\" value=\"$strValue\"$strClassHTML$strDataAttributes>$strButtonInnerArea</button>\n";
			
			return $strInputHTML;
		}

		protected 
		function RenderControlButtonReset($control)
		{
			if (ArrayGetValue($control,"type") != "reset")
			{
				$this->Error("Control is not of reset button type.");	
			}					
			$strLabelHTML = "";
			$strLabel = ArrayGetValue($control,"label");
			$strID = $this->GetControlID($control);
			$strClassHTML = $this->GetClassHTML_Control($control);
			
			$strButtonInnerArea = ReplaceString($this->GetLayoutDef("LAYOUT_CONTROL_BUTTON_INNERAREA"),"{BUTTONLABEL}",$strLabel);
			$strInputHTML = "<button type=\"reset\" id=\"$strID\"$strClassHTML>$strButtonInnerArea</button>\n";
			return $strInputHTML;
		}
		
		protected
		function GetControlID($control)
		{
			return ArrayGetValue($control,"id") . "_".$this->GetFormInstanceToken();	
		}
		
		protected 
		function RenderControl($control,$strMode,&$arrayValues,&$arrayErrors,&$strLabelHTML,&$strControlHTML)
		{
			$strLabelHTML = "";
			$strControlHTML = "";
			
			$strType = ArrayGetValue($control,"type");
			
			$nMaxWidth = intval($this->GetLayoutDef("LAYOUT_CONTROL_".StringUpperCase($strType)."_MAXWIDTH"));
			if ($nMaxWidth > 0)
			{
				$control["width"] = min(ArrayGetValue($control,"width"),$nMaxWidth);
			}
			
			switch ($strType)
			{
			case "text":
				{
					$strLabelHTML = $this->RenderLabel($control,$arrayErrors);
					$strControlHTML = $this->RenderControlText($control,$arrayValues);
					break;
				}		
			case "password":
				{
					$strLabelHTML = $this->RenderLabel($control,$arrayErrors);
					$strControlHTML = $this->RenderControlPassword($control,$arrayValues);
					break;
				}
			case "textarea":
				{
					$strLabelHTML = $this->RenderLabel($control,$arrayErrors);
					$strControlHTML = $this->RenderControlTextArea($control,$arrayValues);
					break;
				}
			case "select":
				{
					$strLabelHTML = $this->RenderLabel($control,$arrayErrors);
					$strControlHTML = $this->RenderControlSelect($control,$arrayValues);
					break;
				}
			case "radio":
				{
					$strLabelHTML = $this->RenderLabel($control,$arrayErrors);
					$strControlHTML = $this->RenderControlRadio($control,$arrayValues);
					break;
				}
			case "checkbox":
				{
					$strLabelHTML = $this->RenderLabel($control,$arrayErrors);
					$strControlHTML = $this->RenderControlCheckbox($control,$arrayValues);
					break;
				}
			case "date":
				{
					$strLabelHTML = $this->RenderLabel($control,$arrayErrors);
					$strControlHTML = $this->RenderControlDate($control,$arrayValues);
					break;
				}
			case "datetime":
				{
					$strLabelHTML = $this->RenderLabel($control,$arrayErrors);
					$strControlHTML = $this->RenderControlDateTime($control,$arrayValues);
					break;
				}
			case "file":
				{
					$strLabelHTML = $this->RenderLabel($control,$arrayErrors);
					$strControlHTML = $this->RenderControlFile($control,$arrayValues);
					break;
				}	
			case "submit":
			case "submitdownload":
				{
					$strLabelHTML = "";
					$strControlHTML = $this->RenderControlButtonSubmit($control);
					break;
				}
			case "reset":
				{
					$strLabelHTML = "";
					$strControlHTML = $this->RenderControlButtonReset($control);
					break;
				}
			case "static":
				{
					$strLabelHTML = "";
					$strControlHTML = ArrayGetValue($control,"payload");
					break;
				}
			case "separator":
				{
					$strLabelHTML = "";
					$strControlHTML = "";
					break;
				}
			case "readonly":
				{
					$strLabelHTML = $this->RenderLabel($control,$arrayErrors);
					$strControlHTML = ArrayGetValue($control,"payload");
					break;
				}
			default:
				{
					$this->Error("Unknown form control type: $strType.");	
				}
			}
			
			//	$strLabelHTML and $strControlHTML are passed by reference to return the result.
		}
		
		protected 
		function LoadIndividualFormLayoutFile($strFormID)
		{
			if ($strFormID == "")
			{
				$this->GetWebsite()->Error("No form id specified.");
			}
			
			$strFile = $this->GetAssemblyLine()->GetFormLayoutFilePath(
				$this->GetWebsite()->GetLayout(),
				$strFormID,
				$this->GetStyle());
		
					
			//$strFile = $strLayoutDir . "layout_" .$this->GetWebsite()->GetLayout()."_form_".$strFormID."_".$this->m_strStyleID.".htm";
			$this->Trace("CElement::LoadIndividualFormLayoutFile: \"$strFile\"");
			if (IsFile($strFile) != true)
			{
				$this->Error("No form layout file found: \"$strFile\"");
			}
			return FileRead($strFile);
	
		}
	}
	
	class CFormElement extends CFormElementHelper
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				"form",
				$strStyleID,
				true,
				false);
		}
		
	
		function RenderForm($formDef, $arrayValues, $arrayErrors = array())
		{
			$this->Trace("RenderForm");
			
			if (!is_object($formDef))
			{
				$this->Error("formDef is not an object.");	
			}
			
			$strErrorText = "";
			if ($formDef->IsValid($strErrorText) != true)
			{
				$this->Error("formDef->IsValid() != true. Error: $strErrorText");	
			}
			
			if (!is_array($arrayValues))
			{
				$this->Error("arrayValues is not an array");	
			}
			
			$this->InitFormInstanceToken();
			
			
			$strOutput = "";
			
			// Visible Framework starts
			$strOutput .= $this->GetLayoutDef("LAYOUT_FORM_BEGIN") . "\n";
			
			
			// Form begins
			$strClassForm = $this->GetClassHTML_Form();
			$strMethod = "post";
			$strOutput .= "<form action=\"". ArrayGetValue($formDef->GetData(),"url")."\" method=\"".$strMethod."\" $strClassForm enctype=\"multipart/form-data\">\n";

			// Start of inner frame
			$strOutput .= $this->GetLayoutDef("LAYOUT_INNER_BEGIN") . "\n";


			// Hidden parameters
			$arrayParams = ArrayGetValue($formDef->GetData(),"params");
			if (!is_array($arrayParams))
			{
				$arrayParams = array();
			}
			$arrayParams = ArrayMerge($this->GetWebsite()->GetSessionAwareArrayVars(),$arrayParams);
			
			
			// Render controls
			
			$arrayControls = ArrayGetValue($formDef->GetData(),"controls");
			
			for ($nIndex = 0; $nIndex < ArrayCount($arrayControls); $nIndex++)
			{
				
				$control = $arrayControls[$nIndex];
				$strType = ArrayGetValue($control,"type");
				$strName = ArrayGetValue($control,"name");
				
				$arrayParams = ArrayRemoveKey($arrayParams,$strName);	
				
				$strSpecialLayoutPostfix = "";
				
				$this->RenderControl($control,"generic",$arrayValues,$arrayErrors,$strLabelHTML,$strInputHTML);
				
				switch ($strType)
				{
									
				case "submit":
				case "reset":
					{
						$strSpecialLayoutPostfix = "_BUTTON";
						break;
					}
				default:
					{
						$strSpecialLayoutPostfix = "_".StringUpperCase($strType);
						break;
					}
				}
				
				$strControlHTML = $this->GetLayoutDef("LAYOUT_CONTROL".$strSpecialLayoutPostfix);
				if ($strControlHTML == "")
				{
					$strControlHTML = $this->GetLayoutDef("LAYOUT_CONTROL");
				}
				$strControlHTML .= "\n";
				
				$strControlHTML = ReplaceString($strControlHTML,"{CONTROL}",$strInputHTML);
				$strControlHTML = ReplaceString($strControlHTML,"{LABEL}",$strLabelHTML);
				$strOutput .= $strControlHTML;

				
			
			}

			// End of inner frame
			$strOutput .= $this->GetLayoutDef("LAYOUT_INNER_END") . "\n";
		
			
			// Hidden parameters
			$strHiddenHTML = "";
			foreach ($arrayParams as $param_name => $param_value)
			{					
				$strHiddenHTML .= "<input type=\"hidden\" name=\"".$param_name."\" value=\"".$param_value."\"/>\n";
			}
			$strOutput .= $strHiddenHTML;

			// End of form		
			$strOutput .= "</form>\n";

			// End of visible framework
			$strOutput .= $this->GetLayoutDef("LAYOUT_FORM_END") . "\n";

			
			return $strOutput;
		}
	}
	
	class CFormTemplateElement extends CFormElementHelper
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				"formtemplate",
				$strStyleID,
				true,
				false);
		}

		function RenderFormTemplate($formDef,$arrayValues,$arrayErrors,$strFormID,$arrayAdditionalItems = array())
		{
			$this->Trace("RenderFormTemplate");
			
			if (!is_object($formDef))
			{
				$this->Error("formDef is not an object.");	
			}
			
			if ($formDef->IsValid() != true)
			{
				$this->Error("formDef->IsValid != true.");	
			}
			
			if (!is_array($arrayValues))
			{
				$this->Error("arrayValues is not an array");	
			}
			
			
			$strFormLayout = $this->LoadIndividualFormLayoutFile($strFormID);
			
			if (is_array($arrayAdditionalItems))
			{
				$strFormLayout = ReplaceTagsWDK($strFormLayout, $arrayAdditionalItems);
			}
			
			$arrayItems = array();
			
			$this->InitFormInstanceToken();
						
			// FORM BEGIN
			$strMethod = "post";
			$strFormBegin = "<form action=\"". ArrayGetValue($formDef->GetData(),"url")."\" method=\"".$strMethod."\" enctype=\"multipart/form-data\">\n";

			// Hidden parameters
			$arrayParams = ArrayGetValue($formDef->GetData(),"params");
			if (!is_array($arrayParams))
			{
				$arrayParams = array();
			}
			$arrayParams = ArrayMerge($this->GetWebsite()->GetSessionAwareArrayVars(),$arrayParams);
			
			
			
		   // Render controls and labels
			
			$arrayControls = ArrayGetValue($formDef->GetData(),"controls");
			
			for ($nIndex = 0; $nIndex < ArrayCount($arrayControls); $nIndex++)
			{ 
				
			
				$control = $arrayControls[$nIndex];
				$strType = ArrayGetValue($control,"type");
				$strName = ArrayGetValue($control,"name");
				$strID = ArrayGetValue($control,"id");
				
				$arrayParams = ArrayRemoveKey($arrayParams,$strName);
				
				$this->RenderControl($control,"individual",$arrayValues,$arrayErrors,$strLabelHTML,$strInputHTML);
				
				$arrayItems[StringUpperCase($strID)."_LABEL"] = $strLabelHTML;
				$arrayItems[StringUpperCase($strID)."_CONTROL"] = $strInputHTML;
				
			
			}
			
			// FORM END
			
			$strFormEnd = "";
			
			// Hidden parameters
			foreach ($arrayParams as $param_name => $param_value)
			{					
				$strFormEnd .= "<input type=\"hidden\" name=\"".$param_name."\" value=\"".$param_value."\"/>\n";
			}
		
			// End of form		
			
			$strFormEnd .= "</form>\n";


			// COMPILE FORM			
			
			$this->SetOutputItems($arrayItems);
			$strOutput = $strFormBegin . $this->ReplaceOutputItems($strFormLayout) . $strFormEnd;
			
			return $strOutput;
		}

	}





