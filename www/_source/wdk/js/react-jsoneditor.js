'use strict';

// A separate draft lets users type incomplete numbers and property names.
class JsonEditorField extends React.Component
{
	constructor(props) { super(props); this.state = { draft: props.value, error: '' }; }
	componentDidUpdate(previous)
	{
		if (previous.value !== this.props.value && !this.focused)
			this.setState({ draft: this.props.value, error: '' });
	}
	commit()
	{
		const error = this.props.onCommit(this.state.draft);
		this.setState({ error: error || '' });
	}
	render()
	{
		return e('span', { className: this.props.className },
			e('input', {
				ref: this.props.inputRef,
				value: this.state.draft, 'aria-label': this.props.label,
				'aria-invalid': !!this.state.error, title: this.state.error,
				onFocus: () => { this.focused = true; },
				onChange: event => { this.setState({ draft: event.target.value, error: '' }); if (this.props.onInput) this.props.onInput(event.target.value); },
				onBlur: () => { this.focused = false; if (!this.skipBlurCommit) this.commit(); this.skipBlurCommit = false; },
				onKeyDown: event => {
					if (event.key === 'Tab' && !event.shiftKey && this.props.onTab)
					{
						event.preventDefault();
						this.skipBlurCommit = true;
						const error = this.props.onTab(this.state.draft);
						if (error) { this.skipBlurCommit = false; this.setState({ error: error }); }
						return;
					}
					if (event.key === 'Enter')
					{
						event.preventDefault();
						if (this.props.onEnter) { this.skipBlurCommit = true; this.props.onEnter(this.state.draft); }
						else this.commit();
					}
					if (event.key === 'Escape') this.setState({ draft: this.props.value, error: '' });
				}
			}),
			this.state.error && e('span', { className: 'jsoneditor-error', role: 'alert' }, this.state.error));
	}
}

class JsonEditorType extends React.Component
{
	render()
	{
		const icons = { string: 'font', number: 'hashtag', boolean: 'toggle-on', null: 'ban', object: 'cube', array: 'list-ul' };
		return e('span', { className: 'jsoneditor-type-picker', title: this.props.value },
			e('span', { className: 'fa fa-' + icons[this.props.value], 'aria-hidden': true }),
			e('select', { value: this.props.value, 'aria-label': this.props.label, onChange: this.props.onChange },
				Object.keys(icons).map(type => e('option', { key: type, value: type }, type))));
	}
}

// Uncommitted rows are UI drafts and never appear in the JSON document.
class JsonEditorNewRow extends React.Component
{
	constructor(props) { super(props); this.state = { name: '', value: '', error: '', type: null }; }
	commit(focusNext = false)
	{
		if (!this.state.name && !this.state.value && this.state.type === null) return;
		const error = this.props.onAdd(this.state.name, this.state.value, this.state.type);
		if (error) this.setState({ error: error });
		else this.setState({ name: '', value: '', error: '', type: null }, () => {
			if (focusNext && this.firstInput) this.firstInput.focus();
		});
	}
	render()
	{
		return e('div', { className: 'jsoneditor-row jsoneditor-new-row', onBlur: event => { if (!event.currentTarget.contains(event.relatedTarget)) this.commit(); }, onKeyDown: event => { if (event.key === 'Enter') { event.preventDefault(); this.commit(true); } } },
			this.props.array ? e('span', {}, 'New item') : e('input', { ref: input => { this.firstInput = input; }, className: 'jsoneditor-key', placeholder: 'New key', 'aria-label': 'New property name', value: this.state.name, onChange: event => this.setState({ name: event.target.value, error: '' }) }),
			e('input', { ref: this.props.array ? input => { this.firstInput = input; } : undefined, className: 'jsoneditor-value', placeholder: 'Value', 'aria-label': 'New value', value: this.state.value, onChange: event => this.setState({ value: event.target.value, error: '', type: null }) }),
			e(JsonEditorType, { label: 'New value type', value: this.state.type || this.props.detectType(this.state.value), onChange: event => {
				const type = event.target.value, defaults = { string: this.state.value, number: '0', boolean: 'false', null: 'null', object: '{}', array: '[]' };
				this.setState({ type: type, value: defaults[type] });
			} }),
			this.state.error && e('span', { className: 'jsoneditor-error', role: 'alert' }, this.state.error));
	}
}

class JsonEditor extends WDKReactComponent
{
	constructor(props)
	{
		super(props);
		this.state = { raw: '{}', data: {}, error: '', rawVisible: true, treeVisible: true, collapsed: {}, revision: 0, split: 50, query: '', searchIndex: -1, activePath: [], line: 1, column: 1 };
	}
	componentDidMount()
	{
		super.componentDidMount();
		this.fullscreenKeyDown = event => { if (event.key === 'Escape' && this.state.fullscreen) this.setFullscreen(false); };
		document.addEventListener('keydown', this.fullscreenKeyDown);
	}
	componentWillUnmount()
	{
		document.removeEventListener('keydown', this.fullscreenKeyDown);
		if (this.state.fullscreen) document.body.style.overflow = this.previousBodyOverflow;
		super.componentWillUnmount();
	}
	setFullscreen(enabled)
	{
		if (enabled === !!this.state.fullscreen) return;
		if (enabled)
		{
			this.previousBodyOverflow = document.body.style.overflow;
			document.body.style.overflow = 'hidden';
		}
		else document.body.style.overflow = this.previousBodyOverflow;
		this.setState({ fullscreen: enabled });
	}
	type(value) { return value === null ? 'null' : Array.isArray(value) ? 'array' : typeof value; }
	// Preserve every character, including incomplete JSON, for the highlighting layer.
	tokens(raw)
	{
		const pattern = /"(?:\\.|[^"\\])*"|-?(?:0|[1-9]\d*)(?:\.\d+)?(?:[eE][+-]?\d+)?|true\b|false\b|null\b|\{\s*\}|\[\s*\]|[{}\[\],:]|\s+|./g;
		const tokens = [];
		let match;
		while ((match = pattern.exec(raw)) !== null)
		{
			const text = match[0], start = match.index;
			let type = 'plain', selectable = false, from = start, to = start + text.length;
			if (text[0] === '"' && text.length > 1)
			{
				type = /^\s*:/.test(raw.slice(to)) ? 'key' : 'string';
				selectable = true; from++; to--;
			}
			else if (/^-?\d/.test(text)) { type = 'number'; selectable = true; }
			else if (/^(true|false|null)$/.test(text)) { type = text === 'null' ? 'null' : 'boolean'; selectable = true; }
			else if (/^[{}\[\],:]/.test(text)) { type = 'punctuation'; selectable = text.length > 1; }
			tokens.push({ text: text, start: start, end: start + text.length, from: from, to: to, type: type, selectable: selectable });
		}
		return tokens;
	}
	syncRawScroll()
	{
		if (this.rawInput && this.rawHighlight)
		{
			this.rawHighlight.scrollTop = this.rawInput.scrollTop;
			this.rawHighlight.scrollLeft = this.rawInput.scrollLeft;
			if (this.lineNumbers) this.lineNumbers.scrollTop = this.rawInput.scrollTop;
		}
	}
	componentDidUpdate() { this.syncRawScroll(); }
	syncEditors(source)
	{
		const raw = this.hasRawFolds() ? this.foldedRaw : this.rawInput;
		const from = source === 'raw' ? raw : this.treePane, to = source === 'raw' ? this.treePane : raw;
		if (source === 'raw' && !this.hasRawFolds()) this.syncRawScroll();
		if (!from || !to || !this.state.rawVisible || !this.state.treeVisible) return;
		const maximum = from.scrollHeight - from.clientHeight;
		const top = maximum > 0 ? from.scrollTop / maximum * Math.max(0, to.scrollHeight - to.clientHeight) : 0;
		if (Math.abs(to.scrollTop - top) > 1) to.scrollTop = top;
	}
	rawRanges()
	{
		const ranges = [], stack = [], paths = {};
		if (!this.state.error) this.sourceRange([], paths);
		let quoted = false, escaped = false, line = 0;
		for (let index = 0; index < this.state.raw.length; index++)
		{
			const character = this.state.raw[index];
			if (character === '\n') line++;
			if (escaped) { escaped = false; continue; }
			if (quoted && character === '\\') { escaped = true; continue; }
			if (character === '"') { quoted = !quoted; continue; }
			if (quoted) continue;
			if (character === '{' || character === '[') stack.push({ start: index, line: line, open: character, path: paths[index] });
			else if (character === '}' || character === ']')
			{
				const range = stack.pop();
				if (range && line > range.line) ranges.push(Object.assign(range, { end: index, endLine: line }));
			}
		}
		return ranges.sort((a, b) => a.start - b.start);
	}
	rawRangeClosed(range)
	{
		return range.path !== undefined ? !!this.state.collapsed[JSON.stringify(range.path)] : !!(this.state.rawCollapsed || {})[range.line];
	}
	hasRawFolds() { return this.rawRanges().some(range => this.rawRangeClosed(range)); }
	toggleBranch(path)
	{
		this.setState(state => ({
			collapsed: Object.assign({}, state.collapsed, { [JSON.stringify(path)]: !state.collapsed[JSON.stringify(path)] }),
			// Give compact branches separate source lines so they can fold too.
			raw: state.error ? state.raw : JSON.stringify(state.data, null, 2)
		}));
	}
	toggleRawFold(line)
	{
		const range = this.rawRanges().find(item => item.line === line);
		if (range && range.path !== undefined) { this.toggleBranch(range.path); return; }
		this.setState(state => ({ rawCollapsed: Object.assign({}, state.rawCollapsed, { [line]: !(state.rawCollapsed || {})[line] }) }));
	}
	rawLines()
	{
		const lines = this.state.raw.split('\n'), ranges = this.rawRanges(), output = [];
		for (let line = 0; line < lines.length; line++)
		{
			const range = ranges.find(item => item.line === line), closed = range && this.rawRangeClosed(range);
			let text = lines[line];
			if (closed)
			{
				const lineStart = this.state.raw.lastIndexOf('\n', range.start) + 1;
				const endStart = this.state.raw.lastIndexOf('\n', range.end) + 1;
				text = text.slice(0, range.start - lineStart + 1) + ' … ' + lines[range.endLine].slice(range.end - endStart);
			}
			output.push({ line: line, text: text, range: range, closed: closed });
			if (closed) line = range.endLine;
		}
		return output;
	}
	editRaw()
	{
		this.setState({ collapsed: {}, rawCollapsed: {} }, () => { if (this.rawInput) this.rawInput.focus(); });
	}
	rawKeyDown(event)
	{
		if (!event.ctrlKey && !event.metaKey && !event.altKey && !event.isComposing)
		{
			const input = event.currentTarget, start = input.selectionStart, end = input.selectionEnd;
			if (!this.insideString(input.value.slice(0, start)))
			{
				if (event.key === '{' || event.key === '[')
				{
					event.preventDefault();
					const closing = event.key === '{' ? '}' : ']';
					this.parse(input.value.slice(0, start) + event.key + input.value.slice(start, end) + closing + input.value.slice(end));
					this.setState({}, () => input.setSelectionRange(start + 1, end + 1));
					return;
				}
				if ((event.key === '}' || event.key === ']') && start === end && input.value[start] === event.key)
				{
					event.preventDefault(); input.setSelectionRange(start + 1, start + 1); return;
				}
			}
		}
		if (event.key === 'Enter' && !event.ctrlKey && !event.metaKey && !event.altKey && !event.isComposing)
		{
			event.preventDefault();
			const input = event.currentTarget, edit = this.indent(input.value, input.selectionStart, input.selectionEnd);
			this.parse(edit.raw);
			this.setState({}, () => { input.setSelectionRange(edit.caret, edit.caret); this.syncRawScroll(); });
			return;
		}
		if (event.key !== 'Tab' || event.ctrlKey || event.metaKey || event.altKey || event.isComposing) return;
		const input = event.currentTarget;
		const tokens = this.tokens(input.value).filter(token => token.selectable);
		const start = input.selectionStart, end = input.selectionEnd;
		const current = tokens.findIndex(token => start >= token.start && end <= token.end && start < token.end);
		let next;
		if (current !== -1) next = tokens[current + (event.shiftKey ? -1 : 1)];
		else if (event.shiftKey) next = tokens.slice().reverse().find(token => token.end <= start);
		else next = tokens.find(token => token.start >= end);
		// At either boundary, normal Tab navigation leaves the editor.
		if (!next) return;
		event.preventDefault();
		input.setSelectionRange(next.from, next.to);
		const before = input.value.slice(0, next.from).split('\n');
		const style = window.getComputedStyle(input);
		const lineHeight = parseFloat(style.lineHeight);
		const top = (before.length - 1) * lineHeight;
		if (top < input.scrollTop || top + lineHeight > input.scrollTop + input.clientHeight)
			input.scrollTop = Math.max(0, top - input.clientHeight / 2);
		const canvas = document.createElement('canvas'), context = canvas.getContext('2d');
		if (context)
		{
			context.font = style.font;
			const left = context.measureText(before[before.length - 1].replace(/\t/g, '    ')).width;
			if (left < input.scrollLeft || left > input.scrollLeft + input.clientWidth - 40)
				input.scrollLeft = Math.max(0, left - input.clientWidth / 2);
		}
		this.syncRawScroll();
	}
	indent(raw, start, end)
	{
		let before = raw.slice(0, start);
		const after = raw.slice(end);
		const line = before.slice(before.lastIndexOf('\n') + 1), base = (line.match(/^[\t ]*/) || [''])[0];
		const unit = (raw.match(/\n(\t+| +)\S/) || [null, '  '])[1];
		const quoted = this.insideString(before);
		if (!quoted && this.needsComma(before) && !/^\s*,/.test(after)) before = before.replace(/\s*$/, ',');
		const opener = !quoted && /[\[{]\s*$/.test(before);
		const indentation = base + (opener ? (unit[0] === '\t' ? '\t' : unit) : '');
		const newline = '\n' + indentation;
		const closing = opener && /^\s*[\]}]/.test(after) ? '\n' + base : '';
		return { raw: before + newline + closing + (closing ? after.replace(/^\s*/, '') : after), caret: before.length + newline.length };
	}
	insideString(text)
	{
		let quoted = false, escaped = false;
		for (const character of text) { if (escaped) escaped = false; else if (character === '\\' && quoted) escaped = true; else if (character === '"') quoted = !quoted; }
		return quoted;
	}
	needsComma(text)
	{
		const stack = [];
		// Track whether the current object/array has just received a complete value.
		const parts = text.match(/"(?:\\.|[^"\\])*"|-?(?:0|[1-9]\d*)(?:\.\d+)?(?:[eE][+-]?\d+)?|true\b|false\b|null\b|[^\s]/g) || [];
		for (const part of parts)
		{
			const current = stack[stack.length - 1];
			if (part === '{' || part === '[') stack.push({ kind: part, expect: part === '{' ? 'key' : 'value' });
			else if (part === '}' || part === ']') { stack.pop(); if (stack.length) stack[stack.length - 1].expect = 'comma'; }
			else if (current)
			{
				if (part === ',') current.expect = current.kind === '{' ? 'key' : 'value';
				else if (part === ':' && current.expect === 'colon') current.expect = 'value';
				else if (part[0] === '"' && current.expect === 'key') current.expect = 'colon';
				else if (current.expect === 'value' && /^(?:"|-?\d|true$|false$|null$)/.test(part)) current.expect = 'comma';
				else current.expect = 'invalid';
			}
		}
		return stack.length > 0 && stack[stack.length - 1].expect === 'comma';
	}
	infer(text)
	{
		try { const value = JSON.parse(text); if (typeof value === 'number' && !Number.isFinite(value)) return text; return value; }
		catch (error) { return text; }
	}
	valueText(value) { return typeof value === 'string' ? value : JSON.stringify(value); }
	parse(raw)
	{
		this.setState({ rawCollapsed: {} });
		try
		{
			const data = JSON.parse(raw, (key, value) => {
				if (typeof value === 'number' && !Number.isFinite(value)) throw new Error('Numbers must be finite.');
				return value;
			});
			this.setState(state => ({ raw: raw, data: data, error: '', collapsed: {}, revision: state.revision + 1 }));
		}
		catch (error) { this.setState({ raw: raw, error: error.message }); }
	}
	get(data, path) { return path.reduce((value, key) => value[key], data); }
	// Define own properties so keys such as __proto__ remain ordinary JSON data.
	put(object, key, value)
	{
		Object.defineProperty(object, key, { value: value, enumerable: true, configurable: true, writable: true });
	}
	change(path, value, remove = false)
	{
		if (this.state.error) return;
		let data = JSON.parse(JSON.stringify(this.state.data));
		if (!path.length) data = remove ? null : value;
		else
		{
			const parent = this.get(data, path.slice(0, -1));
			const key = path[path.length - 1];
			if (remove) { if (Array.isArray(parent)) parent.splice(key, 1); else delete parent[key]; }
			else this.put(parent, key, value);
		}
		this.setState({ data: data, raw: JSON.stringify(data, null, 2), rawCollapsed: {} });
	}
	rename(path, name, focusValue = false)
	{
		const oldName = path[path.length - 1];
		if (name === oldName) { if (focusValue) this.focusRow(path, true); return ''; }
		const parentPath = path.slice(0, -1);
		const parent = this.get(this.state.data, parentPath);
		if (Object.prototype.hasOwnProperty.call(parent, name)) return 'This property already exists.';
		const replacement = {};
		Object.keys(parent).forEach(key => this.put(replacement, key === oldName ? name : key, parent[key]));
		this.change(parentPath, replacement);
		if (focusValue) this.setState({}, () => this.focusRow(parentPath.concat(name), true));
		return '';
	}
	add(path, after = null, name = null, initial = '', focus = false)
	{
		if (this.state.error) return 'Correct the raw JSON before adding a property.';
		const value = JSON.parse(JSON.stringify(this.get(this.state.data, path)));
		if (Array.isArray(value))
		{
			// Plus-created items use the first object's keys, without copying its data.
			if (name === null && initial === '' && this.type(value[0]) === 'object')
			{
				initial = {};
				Object.keys(value[0]).forEach(key => this.put(initial, key, ''));
			}
			value.splice(after === null ? 0 : after + 1, 0, initial);
		}
		else
		{
			if (name === null) { name = 'newKey'; let suffix = 1; while (Object.prototype.hasOwnProperty.call(value, name)) name = 'newKey' + suffix++; }
			if (Object.prototype.hasOwnProperty.call(value, name)) return 'This property already exists.';
			const replacement = {};
			if (after === null) this.put(replacement, name, initial);
			Object.keys(value).forEach(key => { this.put(replacement, key, value[key]); if (key === after) this.put(replacement, name, initial); });
			this.change(path, replacement);
			this.setState(state => ({ collapsed: Object.assign({}, state.collapsed, { [JSON.stringify(path)]: false }) }), () => { if (focus) this.focusRow(path.concat(name)); });
			return '';
		}
		this.change(path, value);
		this.setState(state => ({ collapsed: Object.assign({}, state.collapsed, { [JSON.stringify(path)]: false }) }), () => { if (focus) this.focusRow(path.concat(after === null ? 0 : after + 1)); });
		return '';
	}
	focusRow(path, value = false)
	{
		const input = this.rowInputs && this.rowInputs[JSON.stringify(path) + (value ? ':value' : '')];
		if (input) { input.focus(); input.select(); }
	}
	enterValue(path, draft)
	{
		this.change(path, this.infer(draft));
		this.setState({}, () => {
			if (!path.length) return;
			const parentPath = path.slice(0, -1), parent = this.get(this.state.data, parentPath);
			const keys = Object.keys(parent), index = keys.indexOf(String(path[path.length - 1]));
			if (index + 1 < keys.length)
			{
				const next = keys[index + 1];
				this.focusRow(parentPath.concat(Array.isArray(parent) ? Number(next) : next));
			}
			else
			{
				const row = this.draftRows && this.draftRows[JSON.stringify(parentPath)];
				if (row && row.firstInput) row.firstInput.focus();
			}
		});
	}
	rememberInput(id, input)
	{
		if (!this.rowInputs) this.rowInputs = {};
		this.rowInputs[id] = input;
	}
	setBranches(closed)
	{
		const collapsed = {};
		const visit = (value, path) => {
			if (value === null || typeof value !== 'object') return;
			collapsed[JSON.stringify(path)] = closed;
			Object.keys(value).forEach(key => visit(value[key], path.concat(Array.isArray(value) ? Number(key) : key)));
		};
		visit(this.state.data, []);
		this.setState({ collapsed: collapsed });
	}
	toggleAll()
	{
		const expand = this.hasRawFolds() || Object.values(this.state.collapsed).some(Boolean);
		this.setBranches(!expand);
		// Format valid compact JSON so its branches have foldable source lines.
		this.setState({ raw: this.state.error ? this.state.raw : JSON.stringify(this.state.data, null, 2) }, () => {
			const rawCollapsed = {};
			if (!expand) this.rawRanges().forEach(range => { rawCollapsed[range.line] = true; });
			this.setState({ rawCollapsed: rawCollapsed });
		});
	}
	matchText(text)
	{
		return !!this.state.query && String(text).toLowerCase().includes(this.state.query.toLowerCase());
	}
	highlightToken(token)
	{
		let decoded = token.text;
		if (token.text[0] === '"') { try { decoded = JSON.parse(token.text); } catch (error) {} }
		return e('span', { key: token.start, className: 'jsoneditor-token-' + token.type + (this.matchText(decoded) ? ' jsoneditor-search-hit' : '') }, token.text);
	}
	sourceRange(path, paths)
	{
		const tokens = Array.from(this.state.raw.matchAll(/"(?:\\.|[^"\\])*"|-?(?:0|[1-9]\d*)(?:\.\d+)?(?:[eE][+-]?\d+)?|true|false|null|[{}\[\],:]/g));
		let index = 0, found = null;
		const visit = current => {
			const token = tokens[index++]; if (!token) return;
			const start = token.index;
			if (paths) paths[start] = current;
			if (token[0] === '{')
			{
				while (tokens[index] && tokens[index][0] !== '}') { const key = JSON.parse(tokens[index++][0]); index++; visit(current.concat(key)); if (tokens[index] && tokens[index][0] === ',') index++; }
				index++;
			}
			else if (token[0] === '[')
			{
				let item = 0;
				while (tokens[index] && tokens[index][0] !== ']') { visit(current.concat(item++)); if (tokens[index] && tokens[index][0] === ',') index++; }
				index++;
			}
			if (JSON.stringify(current) === JSON.stringify(path)) found = { start: start, end: tokens[index - 1].index + tokens[index - 1][0].length };
		};
		try { visit([]); } catch (error) { return null; }
		return found;
	}
	findNext(backward = false)
	{
		const query = this.state.query.toLowerCase();
		if (!query) return;
		const matches = [];
		const visit = (value, path) => {
			const label = path.length ? String(path[path.length - 1]) : '';
			if (label.toLowerCase().includes(query) || (value === null || typeof value !== 'object') && String(value).toLowerCase().includes(query)) matches.push(path);
			if (value !== null && typeof value === 'object') Object.keys(value).forEach(key => visit(value[key], path.concat(Array.isArray(value) ? Number(key) : key)));
		};
		visit(this.state.data, []);
		if (!matches.length) { this.setState({ searchMessage: 'No matches' }); return; }
		const index = (this.state.searchIndex + (backward ? -1 : 1) + matches.length) % matches.length, path = matches[index];
		const collapsed = Object.assign({}, this.state.collapsed);
		for (let depth = 0; depth < path.length; depth++) collapsed[JSON.stringify(path.slice(0, depth))] = false;
		this.setState({ searchIndex: index, searchMessage: (index + 1) + ' / ' + matches.length, collapsed: collapsed, rawCollapsed: {}, activePath: path, treeVisible: true }, () => {
			this.focusRow(path, true);
			const range = this.sourceRange(path);
			if (range && this.rawInput)
			{
				const line = this.state.raw.slice(0, range.start).split('\n').length - 1;
				this.rawInput.scrollTop = Math.max(0, line * 21 - this.rawInput.clientHeight / 2);
				this.syncRawScroll();
			}
		});
	}
	moveRow(event)
	{
		if (event.altKey || event.ctrlKey || event.metaKey || event.shiftKey || event.isComposing || event.target.tagName === 'SELECT' || (event.key !== 'ArrowDown' && event.key !== 'ArrowUp')) return;
		const rows = Array.from(event.currentTarget.querySelectorAll('.jsoneditor-row'));
		const current = event.target.closest('.jsoneditor-row'), index = rows.indexOf(current);
		const next = rows[index + (event.key === 'ArrowDown' ? 1 : -1)];
		if (next)
		{
			const column = event.target.closest('.jsoneditor-value') ? 'value' : 'key';
			const input = next.querySelector('input.jsoneditor-' + column + ', .jsoneditor-' + column + ' input') || next.querySelector('input,button');
			if (input) { event.preventDefault(); input.focus(); }
		}
	}
	startResize(event)
	{
		event.preventDefault(); event.currentTarget.setPointerCapture(event.pointerId); this.resizing = true;
	}
	resize(event)
	{
		if (!this.resizing || !this.panes) return;
		const rect = this.panes.getBoundingClientRect();
		this.setState({ split: Math.max(25, Math.min(75, (event.clientX - rect.left) / rect.width * 100)) });
	}
	iconButton(icon, label, action, props = {})
	{
		return this.button(e('span', { className: 'fa fa-' + icon, 'aria-hidden': true }), action, Object.assign({ className: 'jsoneditor-tool', title: label, 'aria-label': label }, props));
	}
	togglePane(pane)
	{
		this.setState(state => {
			const next = { rawVisible: state.rawVisible, treeVisible: state.treeVisible };
			next[pane] = !next[pane];
			if (!next.rawVisible && !next.treeVisible) next[pane === 'rawVisible' ? 'treeVisible' : 'rawVisible'] = true;
			return next;
		});
	}
	button(label, onClick, props = {}) { return e('button', Object.assign({ type: 'button', onClick: onClick }, props), label); }
	node(value, path, parentIsArray)
	{
		const type = this.type(value), branch = type === 'object' || type === 'array';
		const id = JSON.stringify(path), closed = !!this.state.collapsed[id];
		const label = path.length ? String(path[path.length - 1]) : 'root';
		const defaults = { string: '', number: 0, boolean: false, null: null, object: {}, array: [] };
		return e('div', { key: id },
			e('div', { className: 'jsoneditor-row jsoneditor-type-' + type + (JSON.stringify(this.state.activePath) === id ? ' jsoneditor-row-active' : ''), onFocus: () => { if (JSON.stringify(this.state.activePath) !== id) this.setState({ activePath: path }); } },
				branch ? this.iconButton(closed ? 'caret-right' : 'caret-down', (closed ? 'Expand ' : 'Collapse ') + label, () => this.toggleBranch(path), { 'aria-expanded': !closed }) : e('span', { className: 'jsoneditor-leaf-spacer', 'aria-hidden': true }),
				path.length && !parentIsArray ? e(JsonEditorField, { className: 'jsoneditor-key' + (this.matchText(label) ? ' jsoneditor-search-hit' : ''), value: label, label: 'Property name', inputRef: input => this.rememberInput(id, input), onCommit: name => this.rename(path, name), onTab: name => this.rename(path, name, true) }) : e('strong', {}, label),
				e(JsonEditorField, { className: 'jsoneditor-value' + (!branch && this.matchText(this.valueText(value)) ? ' jsoneditor-search-hit' : ''), value: this.valueText(value), label: 'Value of ' + label, inputRef: input => { this.rememberInput(id + ':value', input); if (parentIsArray) this.rememberInput(id, input); }, onInput: draft => this.change(path, this.infer(draft)), onEnter: draft => this.enterValue(path, draft), onCommit: draft => { this.change(path, this.infer(draft)); return ''; } }),
				e(JsonEditorType, { value: type, label: 'Type of ' + label, onChange: event => this.change(path, event.target.value === 'string' ? this.valueText(value) : defaults[event.target.value]) }),
				branch && e('span', { className: 'jsoneditor-count', title: Object.keys(value).length + (type === 'array' ? ' items' : ' properties') }, (type === 'array' ? '[' : '{') + Object.keys(value).length + (type === 'array' ? ']' : '}')),
				branch && this.button(e('span', { className: 'fa fa-' + (type === 'array' ? 'clone' : 'plus'), 'aria-hidden': true }), () => this.add(path, null, null, '', true), { className: 'jsoneditor-add', title: type === 'array' ? 'Create array item from first item’s structure' : 'Add property', 'aria-label': (type === 'array' ? 'Add item to ' : 'Add property to ') + label }),
				path.length > 0 && this.button(e('span', { className: 'fa fa-' + (parentIsArray ? 'clone' : 'plus'), 'aria-hidden': true }), () => this.add(path.slice(0, -1), path[path.length - 1], null, '', true), { className: 'jsoneditor-add', title: parentIsArray ? 'Create array item after ' + label + ' from first item’s structure' : 'Insert after ' + label, 'aria-label': parentIsArray ? 'Create array item after ' + label : 'Insert after ' + label }),
				path.length > 0 && this.button(e('span', { className: 'fa fa-remove', 'aria-hidden': true }), () => this.change(path, null, true), { className: 'jsoneditor-delete', title: 'Delete ' + label, 'aria-label': 'Delete ' + label })),
			branch && !closed && e('div', { className: 'jsoneditor-children' }, Object.keys(value).map(key => this.node(value[key], path.concat(type === 'array' ? Number(key) : key), type === 'array')),
				e(JsonEditorNewRow, { key: 'draft', ref: row => { if (!this.draftRows) this.draftRows = {}; this.draftRows[id] = row; }, array: type === 'array', detectType: text => this.type(this.infer(text)), onAdd: (name, text, selectedType) => {
					const keys = Object.keys(value), after = keys.length ? (type === 'array' ? value.length - 1 : keys[keys.length - 1]) : null;
					return this.add(path, after, name, selectedType === 'string' ? text : this.infer(text));
				} })));
	}
	paneHeader(label, pane)
	{
		const direction = (pane === 'rawVisible') === this.state[pane] ? 'left' : 'right';
		return e('div', { className: 'jsoneditor-pane-header' },
			this.button(e('span', { className: 'fa fa-chevron-' + direction, 'aria-hidden': true }), () => this.togglePane(pane), { className: 'jsoneditor-pane-toggle', title: (this.state[pane] ? 'Hide ' : 'Show ') + label, 'aria-label': (this.state[pane] ? 'Hide ' : 'Show ') + label, 'aria-expanded': this.state[pane] }));
	}
	sharedToolbar()
	{
		const expand = this.hasRawFolds() || Object.values(this.state.collapsed).some(Boolean);
		return e('div', { className: 'jsoneditor-shared-toolbar', role: 'toolbar', 'aria-label': 'JSON editor controls' },
			this.iconButton(expand ? 'expand' : 'compress', expand ? 'Expand all' : 'Collapse all', () => this.toggleAll()),
			this.hasRawFolds() && this.iconButton('pencil', 'Expand folds to edit source', () => this.editRaw()),
			e('input', { className: 'jsoneditor-search', type: 'search', placeholder: 'Find key or value', 'aria-label': 'Find key or value', value: this.state.query, onChange: event => this.setState({ query: event.target.value, searchIndex: -1, searchMessage: '' }), onKeyDown: event => { if (event.key === 'Enter') { event.preventDefault(); this.findNext(event.shiftKey); } } }),
			this.iconButton('search', 'Find next (Enter)', () => this.findNext()),
			e('span', { className: 'jsoneditor-view-controls' },
				this.button('Full screen', () => this.setFullscreen(true), { className: 'jsoneditor-tool jsoneditor-mode', hidden: !!this.state.fullscreen }),
				this.button('Normal view', () => this.setFullscreen(false), { className: 'jsoneditor-tool jsoneditor-mode', hidden: !this.state.fullscreen })));
	}
	render()
	{
		return e('div', { className: 'jsoneditor-editor' + (this.state.fullscreen ? ' jsoneditor-fullscreen' : ''), ref: element => { this.editor = element; } },
			this.sharedToolbar(),
			this.state.error && e('p', { className: 'jsoneditor-error', role: 'alert' }, 'Invalid JSON: ' + this.state.error + ' The tree shows the last valid JSON. Correct the raw text to resume editing.'),
			e('div', { className: 'jsoneditor-panes', ref: element => { this.panes = element; } },
				e('section', { style: this.state.rawVisible ? { flexGrow: this.state.treeVisible ? this.state.split : 1 } : {}, className: 'jsoneditor-pane' + (this.state.rawVisible ? '' : ' jsoneditor-pane-collapsed'), 'aria-label': 'Raw JSON' },
					this.paneHeader('Raw JSON', 'rawVisible'),
					e('div', { className: 'jsoneditor-raw-container', hidden: !this.state.rawVisible },
						this.hasRawFolds() && e('div', { className: 'jsoneditor-folded-raw', tabIndex: 0, 'aria-label': 'Folded JSON source. Double click or press Enter to expand and edit.', ref: element => { this.foldedRaw = element; }, onScroll: () => this.syncEditors('raw'), onDoubleClick: () => this.editRaw(), onKeyDown: event => { if (event.key === 'Enter' && event.target === event.currentTarget) this.editRaw(); } }, this.rawLines().map(item => e('div', { key: item.line, className: 'jsoneditor-code-line' }, e('span', { className: 'jsoneditor-fold-number' }, item.line + 1), item.range ? this.iconButton(item.closed ? 'caret-right' : 'caret-down', (item.closed ? 'Expand' : 'Collapse') + ' line ' + (item.line + 1), () => this.toggleRawFold(item.line)) : e('span', { className: 'jsoneditor-fold-space' }), e('code', {}, this.tokens(item.text).map(token => this.highlightToken(token)))))),
						e('pre', { hidden: this.hasRawFolds(), className: 'jsoneditor-line-numbers', ref: element => { this.lineNumbers = element; } }, this.rawLines().map(item => e('div', { key: item.line }, e('span', { 'aria-hidden': true }, item.line + 1), item.range && this.iconButton('caret-down', 'Collapse line ' + (item.line + 1), () => this.toggleRawFold(item.line))))),
						e('pre', { hidden: this.hasRawFolds(), className: 'jsoneditor-highlight', 'aria-hidden': true, ref: element => { this.rawHighlight = element; } },
							this.tokens(this.state.raw).map(token => this.highlightToken(token)), '\n'),
						e('textarea', { className: 'jsoneditor-raw', hidden: this.hasRawFolds(), value: this.state.raw, wrap: 'off', spellCheck: false, autoCapitalize: 'off', autoCorrect: 'off', 'aria-label': 'Raw JSON. Tab selects the next key or value; Shift+Tab selects the previous one. At either end, Tab leaves the editor.', 'aria-invalid': !!this.state.error, ref: element => { this.rawInput = element; }, onScroll: () => this.syncEditors('raw'), onKeyDown: event => this.rawKeyDown(event), onClick: event => { if (event.currentTarget.value === '{}') event.currentTarget.select(); }, onBlur: () => { if (!this.state.error) this.setState({ raw: JSON.stringify(this.state.data, null, 2) }); }, onChange: event => this.parse(event.target.value) }))),
				this.state.rawVisible && this.state.treeVisible && e('div', { className: 'jsoneditor-divider', role: 'separator', tabIndex: 0, 'aria-label': 'Resize editor panes', 'aria-orientation': 'vertical', 'aria-valuemin': 25, 'aria-valuemax': 75, 'aria-valuenow': Math.round(this.state.split), onPointerDown: event => this.startResize(event), onPointerMove: event => this.resize(event), onPointerUp: () => { this.resizing = false; }, onLostPointerCapture: () => { this.resizing = false; }, onKeyDown: event => { if (event.key === 'ArrowLeft' || event.key === 'ArrowRight') { event.preventDefault(); this.setState({ split: Math.max(25, Math.min(75, this.state.split + (event.key === 'ArrowLeft' ? -5 : 5))) }); } } } ),
				e('section', { style: this.state.treeVisible ? { flexGrow: this.state.rawVisible ? 100 - this.state.split : 1 } : {}, className: 'jsoneditor-pane' + (this.state.treeVisible ? '' : ' jsoneditor-pane-collapsed'), 'aria-label': 'JSON tree' },
					this.paneHeader('Tree editor', 'treeVisible'), e('div', { className: 'jsoneditor-tree', ref: element => { this.treePane = element; }, onScroll: () => this.syncEditors('tree'), hidden: !this.state.treeVisible, onKeyDown: event => this.moveRow(event) }, e('fieldset', { disabled: !!this.state.error, key: this.state.revision }, this.node(this.state.data, [], false))))),
			e('div', { className: 'jsoneditor-status', role: 'status' }, e('span', {}, this.state.error ? 'Invalid JSON' : 'Valid JSON'), e('span', { className: 'jsoneditor-path' }, '$' + this.state.activePath.map(key => '[' + JSON.stringify(key) + ']').join('')), e('span', {}, this.state.searchMessage || '↑ / ↓  Navigate rows')));
	}
}
