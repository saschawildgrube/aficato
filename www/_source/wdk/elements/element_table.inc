<?php

	class CTableElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				"table",
				$strStyleID,
				true,
				false);
		}
		
		function RenderTable($arrayTableContent, $bHeaderFirstRow = false, $bHeaderFirstCol = false)
		{
			$this->Trace("bHeaderFirstRow ".$bHeaderFirstRow);
			$this->Trace("bHeaderFirstCol ".$bHeaderFirstCol);
			$strOutput = "";
			if (!is_array($arrayTableContent))
			{
				$this->Error("RenderTable: Table content is empty.");	
				return;
			}
			$strOutput .= $this->GetLayoutDef("LAYOUT_TABLE_BEGIN") . "\n";
		
			reset($arrayTableContent);
			
			for ($nRowIndex = 0; $nRowIndex < ArrayCount($arrayTableContent); $nRowIndex++)
			{
				$arrayColumns = current($arrayTableContent);
				
				if (!is_array($arrayColumns))
				{
					$this->Error("RenderTable: No array in row $nRowIndex.");
					return;	
				}
				reset($arrayColumns);
				
				$nColumnsCount = ArrayCount($arrayColumns);
				
				$strOutput .= $this->GetLayoutDef("LAYOUT_TABLE_ROW_BEGIN") . "\n";
				for ($nColIndex = 0; $nColIndex < $nColumnsCount; $nColIndex++)
				{
					$strCellBeginID = "LAYOUT_TABLE_DATACELL_BEGIN";
					$strCellEndID = "LAYOUT_TABLE_DATACELL_END";
					if ((($nColIndex == 0) && ($bHeaderFirstCol == true)) || (($nRowIndex == 0) && ($bHeaderFirstRow == true)))
					{
						$strCellBeginID = "LAYOUT_TABLE_HEADERCELL_BEGIN";
						$strCellEndID = "LAYOUT_TABLE_HEADERCELL_END";
					}
					$strOutput .= $this->GetLayoutDef($strCellBeginID); // Removed the new line here, because when using the table element to render a table in a Word xml document, the new line is interpreted as a blank. F*** MS.
					$vCurrent = current($arrayColumns);
					if (is_array($vCurrent))
					{
						$vCurrent = ArrayRender($vCurrent); 
					}
					$strOutput .= $vCurrent;
					$strOutput .= $this->GetLayoutDef($strCellEndID) . "\n";
					next($arrayColumns);
				}
				$strOutput .= $this->GetLayoutDef("LAYOUT_TABLE_ROW_END") . "\n";
				
				next($arrayTableContent);
			}
			$strOutput .= $this->GetLayoutDef("LAYOUT_TABLE_END") . "\n";
			return $strOutput;
		}


		function RenderTableEx($arrayTableContent, $arrayTableHeaders, $arrayCellStyle = array())
		{
			$strOutput = "";
			if (!is_array($arrayTableContent))
			{
				$this->Error("RenderTable: Table content is empty.");	
				return;
			}
			$strOutput .= $this->GetLayoutDef("LAYOUT_TABLE_BEGIN") . "\n";
		
			reset($arrayTableContent);
			
			for ($nRowIndex = 0; $nRowIndex < ArrayCount($arrayTableContent); $nRowIndex++)
			{
				$arrayColumns = current($arrayTableContent);
				
				if (!is_array($arrayColumns))
				{
					$this->Error("RenderTable: No array in row $nRowIndex.");
					return;	
				}
				reset($arrayColumns);
				
				$nColumnsCount = ArrayCount($arrayColumns);
				$strOutput .= $this->GetLayoutDef("LAYOUT_TABLE_ROW_BEGIN") . "\n";
				
				for ($nColIndex = 0; $nColIndex < $nColumnsCount; $nColIndex++)
				{
					$strCellBeginID = "LAYOUT_TABLE_DATACELL_BEGIN";
					$strCellEndID = "LAYOUT_TABLE_DATACELL_END";
					if (isset($arrayTableHeaders[$nRowIndex][$nColIndex]) == true)   
					{
						if ($arrayTableHeaders[$nRowIndex][$nColIndex] != false)
						{
							$strCellBeginID = "LAYOUT_TABLE_HEADERCELL_BEGIN";
							$strCellEndID = "LAYOUT_TABLE_HEADERCELL_END";
						}
					}
					$strCellStyle = StringUpperCase(ArrayGetValue($arrayCellStyle,$nRowIndex,$nColIndex));
					if ($strCellStyle != "" && $strCellStyle != "LEFT")
					{
							$strCellBeginID .= "_".$strCellStyle;
					}
					$strCellBeginHTML = $this->GetLayoutDef($strCellBeginID); 
					$strOutput .= $strCellBeginHTML;  // Removed the new line here, because when using the table element to render a table in a Word xml document, the new line is interpreted as a blank. F*** MS.
					$vCurrent = current($arrayColumns);
					if (is_array($vCurrent))
					{
						$vCurrent = ArrayRender($vCurrent); 
					}
					$strOutput .= $vCurrent;
					$strOutput .= $this->GetLayoutDef($strCellEndID) . "\n";
					next($arrayColumns);
				}
				$strOutput .= $this->GetLayoutDef("LAYOUT_TABLE_ROW_END") . "\n";
				
				next($arrayTableContent);
			}
			$strOutput .= $this->GetLayoutDef("LAYOUT_TABLE_END") . "\n";
			return $strOutput;
		}



	}

