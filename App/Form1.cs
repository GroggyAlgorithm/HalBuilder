using System;
using System.Collections.Generic;
using System.Collections.Immutable;
using System.Diagnostics;

namespace HalBuilder;

public partial class Form1 : Form
{
    
    static readonly string pythonPath = Directory.GetCurrentDirectory() + "/py/venv/Scripts/python.exe";
    static readonly string pythonCmd = Directory.GetCurrentDirectory() + "/py/venv/builder.py";
    
    /// <summary>
    /// The main display layout panel
    /// </summary>
    private FlowLayoutPanel mainDisplay;
    
    /// <summary>
    /// The text box that displays the path to the definitions file
    /// </summary>
    private TextBox tbDefinitionsFile;

    /// <summary>
    /// The text box that displays the path to the CONFIG definitions files
    /// </summary>
    private TextBox tbConfigDefinitionsFile;
    
    /// <summary>
    /// The textbox that displays the path to the output file
    /// </summary>
    private TextBox tbOutputFile;

    /// <summary>
    /// Name of signed author
    /// </summary>
    private string authorName = "Auto Builder: Tim Robbins";


    private class InputBoxDialog : Form
    {
        public string  Value  { get { return _txtInput.Text; } }

        private Label    _lblPrompt;
        private TextBox  _txtInput;
        private Button   _btnOk;
        private Button   _btnCancel;

        public InputBoxDialog(string prompt, string title, string defaultValue = null, int? xPos = null, int? yPos = null)
        {
            if (xPos == null && yPos == null)
            {
                StartPosition = FormStartPosition.CenterParent;
            }
            else
            {
                StartPosition = FormStartPosition.Manual;

                if (xPos == null)  xPos = (Screen.PrimaryScreen.WorkingArea.Width  - Width ) >> 1;
                if (yPos == null)  yPos = (Screen.PrimaryScreen.WorkingArea.Height - Height) >> 1;

                Location = new Point(xPos.Value, yPos.Value);
            }

            InitializeComponent();

            if (title == null)  title = Application.ProductName;
            Text = title;

            _lblPrompt.Text = prompt;
            Graphics  graphics = CreateGraphics();
            _lblPrompt.Size = graphics.MeasureString(prompt, _lblPrompt.Font).ToSize();
            int  promptWidth  = _lblPrompt.Size.Width;
            int  promptHeight = _lblPrompt.Size.Height;

            _txtInput.Location  = new Point(8, 30 + promptHeight);
            int inputWidth = promptWidth < 206 ? 206 : promptWidth;
            _txtInput.Size      = new Size(inputWidth, 21);
            _txtInput.Text      = defaultValue;
            _txtInput.SelectAll();
            _txtInput.Focus();

            Height = 110 + promptHeight;
            Width  = inputWidth + 15;

            _btnOk.Location = new Point(8, 60 + promptHeight);
            _btnOk.Size     = new Size(100, 26);

            _btnCancel.Location = new Point(114, 60 + promptHeight);
            _btnCancel.Size     = new Size(100, 26);

            this.ClientSize = new System.Drawing.Size(Width, Height);

            FormBorderStyle = FormBorderStyle.FixedDialog;
            MaximizeBox = false;
            MinimizeBox = false;

            return;
        }


        

        protected void  InitializeComponent()
        {
            _lblPrompt           = new Label();
            _lblPrompt.Location  = new Point(12, 9);
            _lblPrompt.TabIndex  = 0;
            _lblPrompt.BackColor = Color.Transparent;

            _txtInput          = new TextBox();
            _txtInput.Size     = new Size(156, 20);
            _txtInput.TabIndex = 1;

            _btnOk              = new Button();
            _btnOk.TabIndex     = 2;
            _btnOk.Size         = new Size(75, 26);
            _btnOk.Text         = "&OK";
            _btnOk.DialogResult = DialogResult.OK;
            _btnOk.BackColor = Color.Green;

            _btnCancel              = new Button();
            _btnCancel.TabIndex     = 3;
            _btnCancel.Size         = new Size(75, 26);
            _btnCancel.Text         = "&Cancel";
            _btnCancel.BackColor = Color.Red;
            _btnCancel.DialogResult = DialogResult.Cancel;

            AcceptButton = _btnOk;
            CancelButton = _btnCancel;

            Controls.Add(_lblPrompt);
            Controls.Add(_txtInput);
            Controls.Add(_btnOk);
            Controls.Add(_btnCancel);

            

            return;
        }
    }


    private class MultiInputBoxDialog : Form
    {
        private string _value;
        public string Value 
        { 
            get 
            {
                _value = "";
                _value = ReadInputsToString();
                return _value;
            } 
        }
        public List<string> ValueList
        { 
            get 
            {
                return ReadInputsToLines();
            } 
        }
        public int Count { get { return _txtInputs.Length; } }
        protected int _count;
        protected Dictionary<TextBox, string>? _boxPrompts;
        protected Label    _lblPrompt;
        protected TextBox  _txtCountInput;
        protected TextBox[] _txtInputs;
        protected int _inputBoxEndingY;
        protected Button   _btnAdd;
        protected Button   _btnRemove;
        protected Button   _btnOk;
        protected Button   _btnCancel;
        protected Graphics graphics;

        public MultiInputBoxDialog(string prompt, string title, int? boxCount = 0, bool lockCount = false, int? xPos = null, int? yPos = null)
        {
            _inputBoxEndingY = 0;
            _value = "";
            _txtInputs = new TextBox[0];
            _btnCancel = new Button();
            _btnOk = new Button();
            _btnAdd = new Button();
            _btnRemove = new Button();
            _txtCountInput = new TextBox();
            _lblPrompt = new Label();

            if (xPos == null && yPos == null)
            {
                StartPosition = FormStartPosition.CenterParent;
            }
            else
            {
                StartPosition = FormStartPosition.Manual;

                if (xPos == null)  xPos = (Screen.PrimaryScreen.WorkingArea.Width  - Width ) >> 1;
                if (yPos == null)  yPos = (Screen.PrimaryScreen.WorkingArea.Height - Height) >> 1;

                Location = new Point(xPos.Value, yPos.Value);
            }

            if (title == null)  title = Application.ProductName;
            Text = title;

            _count = (int)((boxCount != null && boxCount >= 0) ? boxCount : 0);
            InitializeComponent();

            _lblPrompt.Text = prompt;
            graphics = CreateGraphics();
            _lblPrompt.Size = graphics.MeasureString(prompt, _lblPrompt.Font).ToSize();
            int  promptWidth  = _lblPrompt.Size.Width;
            int  promptHeight = _lblPrompt.Size.Height;
            int inputWidth = promptWidth < 206 ? 206 : promptWidth;

            _txtCountInput.Location  = new Point(8, 30 + promptHeight);
            _txtCountInput.Size      = new Size(inputWidth, 21);
            if(lockCount)
            {
                _txtCountInput.Enabled = false;
                _btnAdd.Enabled = false;
                _btnRemove.Enabled = false;
            }
            
            
            
            Height = 110 + promptHeight;
            Width  = inputWidth + 15;

            SetTextBoxes(boxCount);
            Controls.Add(_txtCountInput);
            Controls.Add(_lblPrompt);
            Controls.Add(_btnOk);
            Controls.Add(_btnCancel);
            Controls.Add(_btnAdd);
            Controls.Add(_btnRemove);

            this.ClientSize = new System.Drawing.Size(Width, Height);

            FormBorderStyle = FormBorderStyle.FixedDialog;
            
            MaximizeBox = false;
            MinimizeBox = false;

            return;
        }


        public MultiInputBoxDialog(string prompt, string title, IList<string> boxPrompts, int? xPos = null, int? yPos = null)
        {
            _inputBoxEndingY = 0;
            _value = "";
            _txtInputs = new TextBox[0];
            _btnCancel = new Button();
            _btnOk = new Button();
            _btnRemove = new Button();
            _btnAdd = new Button();
            _txtCountInput = new TextBox();
            _lblPrompt = new Label();

            if (xPos == null && yPos == null)
            {
                StartPosition = FormStartPosition.CenterParent;
            }
            else
            {
                StartPosition = FormStartPosition.Manual;

                if (xPos == null)  xPos = (Screen.PrimaryScreen.WorkingArea.Width  - Width ) >> 1;
                if (yPos == null)  yPos = (Screen.PrimaryScreen.WorkingArea.Height - Height) >> 1;

                Location = new Point(xPos.Value, yPos.Value);
            }

            if (title == null)  title = Application.ProductName;
            Text = title;

            _count = (boxPrompts.Count >= 0) ? boxPrompts.Count : 0;
            InitializeComponent();

            _lblPrompt.Text = prompt;
            graphics = CreateGraphics();
            _lblPrompt.Size = graphics.MeasureString(prompt, _lblPrompt.Font).ToSize();
            int  promptWidth  = _lblPrompt.Size.Width;
            int  promptHeight = _lblPrompt.Size.Height;
            int inputWidth = promptWidth < 206 ? 206 : promptWidth;

            _txtCountInput.Location  = new Point(8, 30 + promptHeight);
            _txtCountInput.Size      = new Size(inputWidth, 21);
            _txtCountInput.Enabled = false;
            _btnAdd.Enabled = false;
            _btnRemove.Enabled = false;
            
            
            Height = 110 + promptHeight;
            Width  = inputWidth + 15;

            SetTextBoxes(_count,(int)(_txtCountInput.Width/2));
            int largestWidth = 0;
            if(boxPrompts.Count > 0)
            {
                _boxPrompts = new Dictionary<TextBox, string>();
            }
            for(var i = 0; i < boxPrompts.Count; i++)
            {
                var p = boxPrompts[i];
                var lb = new Label();
                lb.Text = p;
                int locationX = _lblPrompt.Location.X;
                int locationY = _txtCountInput.Bottom + 10;
                lb.Width = graphics.MeasureString(p, lb.Font).ToSize().Width+5;
                if(lb.Width > largestWidth)
                {
                    largestWidth = lb.Width;
                }

                if(i > 0)
                {
                    locationY = _txtInputs[i - 1].Bottom + 1;
                }
                
                lb.Location = new Point(locationX, locationY);
                if(i < _txtInputs.Length)
                {
                    _boxPrompts.TryAdd(_txtInputs[i], p);
                }
                
                Controls.Add(lb);
            }

            SetTextBoxes(_count,(int)(_txtCountInput.Width - largestWidth + 10),15);
            Width = _txtInputs[0].Right + 15;//largestWidth + 10 + _txtInputs[0].Width+15;// + inputWidth + 15 
            Controls.Add(_txtCountInput);
            Controls.Add(_lblPrompt);
            Controls.Add(_btnOk);
            Controls.Add(_btnCancel);
            Controls.Add(_btnAdd);
            Controls.Add(_btnRemove);

            this.ClientSize = new System.Drawing.Size(Width, Height);

            FormBorderStyle = FormBorderStyle.FixedDialog;
            MaximizeBox = false;
            MinimizeBox = false;
            // WindowState = FormWindowState.Maximized;
            return;
        }


        protected virtual void SetTextBoxes(int? newCount = null, int? width = null, int xOffset=10)
        {
            int boxWidth = (int)((width != null && width >= 0) ? width : _txtCountInput.Width);
            int startingX = (int)((width != null && width >= 0) ? width + _txtCountInput.Location.X + xOffset: _txtCountInput.Location.X);
            int startngY = _txtCountInput.Bottom + 10;

            //Try to get our set count
            if(newCount != null)
            {
                _count = (int)((newCount >= 0) ? newCount : 0);
            }
            else
            {
                if(int.TryParse(_txtCountInput.Text,out var buffSize))
                {
                    _count = (buffSize >= 0) ? buffSize : 0;
                }
                else
                {
                    _count = 0;
                }
            }

            //Create a list for our currently existing data
            List<string> previousData = new List<string>();

            //Loop through each text box, allowing us to dispose of the old ones after saving the data
            for(var i = 0; i < _txtInputs.Count(); i++)
            {
                var tb = _txtInputs[i];
                previousData.Add(tb.Text);
                if(Controls.Contains(tb))
                {
                    Controls.Remove(tb);
                }
                tb.Dispose();
            }

            //Reinitialize the text box input array
            _txtInputs = new TextBox[(_count >= 0) ? _count : 0];
            
            //Loop through each text box input item...
            for(int i = 0; i < _txtInputs.Count(); i++)
            {
                //Create a new text box
                var tb = new TextBox();

                if(i > 0)
                {
                    
                    tb.Location = new Point(startingX,_txtInputs[i-1].Bottom+1);
                }
                else
                {
                    tb.Location = new Point(startingX,startngY);
                }
                
                tb.Width = boxWidth;

                if(i < previousData.Count)
                {
                    tb.Text = previousData[i];
                }
                
                Controls.Add(tb);
                _txtInputs[i] = tb;
            }

            _inputBoxEndingY = (_txtInputs.Length > 0) ? _txtInputs[_txtInputs.Length-1].Bottom+1 : startngY;
            _btnAdd.Location = new Point(8, _inputBoxEndingY + 10 + _lblPrompt.Size.Height + _txtCountInput.Size.Height);
            _btnRemove.Location = new Point(114, _inputBoxEndingY + 10 + _lblPrompt.Size.Height + _txtCountInput.Size.Height);
            _btnOk.Location = new Point(8, _inputBoxEndingY + 10 + _lblPrompt.Size.Height + _txtCountInput.Size.Height + _btnAdd.Height + _btnRemove.Height);
            _btnCancel.Location = new Point(114, _inputBoxEndingY + 10 + _lblPrompt.Size.Height + _txtCountInput.Size.Height + _btnAdd.Height + _btnRemove.Height);
            Height = 10 + _lblPrompt.Height + _inputBoxEndingY + _lblPrompt.Size.Height + _txtCountInput.Size.Height + _btnOk.Height + _btnCancel.Height + _btnAdd.Height + _btnRemove.Height;
        }

        protected virtual void SetTextBoxes(ref TextBox[] _txtInputs, int? width = null, int xOffset=10, int? newCount = null)
        {
            int boxWidth = (int)((width != null && width >= 0) ? width : _txtCountInput.Width);
            int startingX = (int)((width != null && width >= 0) ? width + _txtCountInput.Location.X + xOffset: _txtCountInput.Location.X);
            int startngY = _txtCountInput.Bottom + 10;

            //Try to get our set count
            if(newCount != null)
            {
                _count = (int)((newCount >= 0) ? newCount : 0);
            }
            else
            {
                if(int.TryParse(_txtCountInput.Text,out var buffSize))
                {
                    _count = (buffSize >= 0) ? buffSize : 0;
                }
                else
                {
                    _count = 0;
                }
            }

            //Create a list for our currently existing data
            List<string> previousData = new List<string>();

            //Loop through each text box, allowing us to dispose of the old ones after saving the data
            for(var i = 0; i < _txtInputs.Count(); i++)
            {
                var tb = _txtInputs[i];
                previousData.Add(tb.Text);
                if(Controls.Contains(tb))
                {
                    Controls.Remove(tb);
                }
                tb.Dispose();
            }

            //Reinitialize the text box input array
            _txtInputs = new TextBox[(_count >= 0) ? _count : 0];
            
            //Loop through each text box input item...
            for(int i = 0; i < _txtInputs.Count(); i++)
            {
                //Create a new text box
                var tb = new TextBox();

                if(i > 0)
                {
                    
                    tb.Location = new Point(startingX,_txtInputs[i-1].Bottom+1);
                }
                else
                {
                    tb.Location = new Point(startingX,startngY);
                }
                
                tb.Width = boxWidth;

                if(i < previousData.Count)
                {
                    tb.Text = previousData[i];
                }
                
                Controls.Add(tb);
                _txtInputs[i] = tb;
            }

        }

        protected virtual void SetHeightsAndLocations(int startngY)
        {
            _inputBoxEndingY = (_txtInputs.Length > 0) ? _txtInputs[_txtInputs.Length-1].Bottom+1 : startngY;
            _btnAdd.Location = new Point(8, _inputBoxEndingY + 10 + _lblPrompt.Size.Height + _txtCountInput.Size.Height);
            _btnRemove.Location = new Point(114, _inputBoxEndingY + 10 + _lblPrompt.Size.Height + _txtCountInput.Size.Height);
            _btnOk.Location = new Point(8, _inputBoxEndingY + 10 + _lblPrompt.Size.Height + _txtCountInput.Size.Height + _btnAdd.Height + _btnRemove.Height);
            _btnCancel.Location = new Point(114, _inputBoxEndingY + 10 + _lblPrompt.Size.Height + _txtCountInput.Size.Height + _btnAdd.Height + _btnRemove.Height);
            Height = 10 + _lblPrompt.Height + _inputBoxEndingY + _lblPrompt.Size.Height + _txtCountInput.Size.Height + _btnOk.Height + _btnCancel.Height + _btnAdd.Height + _btnRemove.Height;
        }

        
        void IntTextBoxKeyPress(object? sender, KeyPressEventArgs e)
        {
            if(sender == null)
            {
                return;
            }
            else if(sender.GetType() == typeof(TextBox))
            {
                var tb =  (TextBox)sender;
                if (!char.IsControl(e.KeyChar) && !char.IsDigit(e.KeyChar))
                {
                    e.Handled = true;
                }
            }
            
        }
        

        protected void  InitializeComponent()
        {
            _lblPrompt           = new Label();
            _lblPrompt.Location  = new Point(10, 9);
            _lblPrompt.TabIndex  = 0;
            _lblPrompt.BackColor = Color.Transparent;

            _txtCountInput = new TextBox();
            _txtCountInput.Size     = new Size(156, 20);
            _txtCountInput.TabIndex = 1;
            _txtCountInput.MaxLength = 3;
            _txtCountInput.Text = _count.ToString();
            _txtCountInput.KeyPress += IntTextBoxKeyPress;
            _txtCountInput.TextChanged += new EventHandler((_,_)=>SetTextBoxes());

            void AddCount()
            {
                _count = (_count < 999) ? _count + 1 : 999;
                _txtCountInput.Text = _count.ToString();
            }

            void RemoveCount()
            {
                _count = (_count > 0) ? _count - 1 : 0;
                _txtCountInput.Text = _count.ToString();
            }

            _btnAdd              = new Button();
            _btnAdd.TabIndex     = 2;
            _btnAdd.Size     = new Size(100, 26);
            _btnAdd.Text         = "&Add";
            _btnAdd.BackColor = Color.Green;
            _btnAdd.Click += new EventHandler((_, _) => AddCount());

            _btnRemove              = new Button();
            _btnRemove.TabIndex     = 3;
            _btnRemove.Size     = new Size(100, 26);
            _btnRemove.Text         = "&Remove";
            _btnRemove.BackColor = Color.Red;
            _btnRemove.Click += new EventHandler((_, _) => RemoveCount());


            _btnOk              = new Button();
            _btnOk.TabIndex     = 2;
            _btnOk.Size     = new Size(100, 26);
            _btnOk.Text         = "&OK";
            _btnOk.DialogResult = DialogResult.OK;
            _btnOk.BackColor = Color.Green;


            _btnCancel              = new Button();
            _btnCancel.TabIndex     = 3;
            _btnCancel.Size     = new Size(100, 26);
            _btnCancel.Text         = "&Cancel";
            _btnCancel.BackColor = Color.Red;
            _btnCancel.DialogResult = DialogResult.Cancel;

            AcceptButton = _btnOk;
            CancelButton = _btnCancel;
            return;
        }
    
        public virtual string ReadInputsToString()
        {
            string? val = "";
            foreach(var box in _txtInputs)
            {
                val += box.Text + "\n";
            }
            return val;
        }

        public virtual List<string> ReadInputsToLines()
        {
            List<string> data = new List<string>();
            if(_boxPrompts != null)
            {
                foreach(var box in _txtInputs)
                {
                    if(_boxPrompts.TryGetValue(box,out var t))
                    {
                        data.Add(t + box.Text);
                    }
                    else
                    {
                        data.Add(box.Text);
                    }
                    
                }
            }
            else
            {
                foreach(var box in _txtInputs)
                {
                    data.Add(box.Text);
                }
            }
            return data;
            
        }
    
    }

    private int RunPyCmd(string cmd, string args, string? pythonPath = null)
    {
        try
        { 
            ProcessStartInfo start = new ProcessStartInfo();

            //If a path was passed...
            if(pythonPath != null)
            {
                if(File.Exists(pythonPath))
                {
                    start.FileName = pythonPath;
                }
            }
            //else...
            else
            {
                start.FileName = Directory.GetCurrentDirectory() + "/py/venv/Scripts/python.exe ";
                if(File.Exists(start.FileName) == false)
                {
                    MessageBox.Show("Could not find python\n", "):", MessageBoxButtons.OK,MessageBoxIcon.Error);
                    return 1;
                }
            }
            start.Arguments = string.Format("{0} {1}", cmd, args);
            start.UseShellExecute = false;
            start.RedirectStandardOutput = true;
            string resultText = "\n";
            using (Process process = Process.Start(start))
            {
                using (StreamReader reader = process.StandardOutput)
                {
                    string result = reader.ReadToEnd();
                    resultText += result + "\n";
                    Console.Write(result);
                }
            }

            MessageBox.Show(resultText, tbOutputFile.Text,MessageBoxButtons.OK,MessageBoxIcon.Information);
        }
        catch (Exception ex)
        {
            MessageBox.Show("Could not generate file\n"+ex.Message, "):",MessageBoxButtons.OK,MessageBoxIcon.Error);
            return 1;
        }


        return 0;

    }

    private void OpenAuthorName()
    {
        using (InputBoxDialog form = new InputBoxDialog(prompt: "", title: "Enter Author Name", defaultValue: authorName))
        {
            if (form.ShowDialog() == DialogResult.OK)
            {
                authorName = form.Value;
                this.Text = "Auto Builder - " + authorName;
            }
        }
        
    }

    struct Command
    {
        public string printableName;
        public string textValue;
        public int? textBoxItems;
        public bool lockTextBox;
    };

    static readonly List<Command> _commands = new List<Command>()
    {
        new Command(){printableName = "Remove Sequences",textValue = "REMOVE",textBoxItems = null,lockTextBox = false},
        new Command(){printableName = "Custom Definitions",textValue = "CUSTOM_DEF",textBoxItems = null,lockTextBox = false},
        new Command(){printableName = "Peripheral Definitions",textValue = "PERIPHERAL_DEF",textBoxItems = null,lockTextBox = false},
        new Command(){printableName = "Definitions",textValue = "DEFS",textBoxItems = null,lockTextBox = false},
        new Command(){printableName = "Custom Section",textValue = "CUSTOM_SECTION",textBoxItems = null,lockTextBox = false},
        new Command(){printableName = "Range Item Prefix",textValue = "ORDERED_PREFIX",textBoxItems = 1,lockTextBox = true},
        new Command(){printableName = "Range Item Suffix",textValue = "ORDERED_SUFFIX",textBoxItems = 1,lockTextBox = true},
        new Command(){printableName = "Range Start",textValue = "DEF_RANGE_START",textBoxItems = 1,lockTextBox = true},
        new Command(){printableName = "Range End",textValue = "DEF_RANGE_END",textBoxItems = 1,lockTextBox = true},
        new Command(){printableName = "Definition Syntax",textValue = "DEF_SYNTAX",textBoxItems = 1,lockTextBox = true},
        new Command(){printableName = "Prefixes",textValue = "PREFIX",textBoxItems = null,lockTextBox = false},
        new Command(){printableName = "Suffixes",textValue = "SUFFIX",textBoxItems = null,lockTextBox = false},
        new Command(){printableName = "Replace Values",  textValue = "REPLACE",     textBoxItems = null,lockTextBox = false},
        new Command(){printableName = "Ignore Sequences",textValue = "IGNORE_LINE", textBoxItems = null,lockTextBox = false},
        new Command(){printableName = "Add After Header",textValue = "ADD_TOP_NEXT",textBoxItems = null,lockTextBox = false},
        new Command(){printableName = "Add After Footer",textValue = "ADD_END_NEXT",textBoxItems = null,lockTextBox = false},
        new Command(){printableName = "Add Before Header",textValue = "ADD_TOP_PRE",textBoxItems = null,lockTextBox = false},
        new Command(){printableName = "Add Before Footer",textValue = "ADD_END_PRE",textBoxItems = null,lockTextBox = false},
    };

    List<string> Commands
    {
        get
        {
            List<string> tmp = new List<string>();
            foreach (var item in _commands)
            {
                tmp.Add(item.textValue);
            }
            return tmp;
        }
    }

    List<string> PrettyCommands
    {
        get
        {
            List<string> tmp = new List<string>();
            foreach (var item in _commands)
            {
                tmp.Add(item.printableName);
            }
            return tmp;
        }
    }

    Dictionary<ToolStripMenuItem, Command> cmdTools = new Dictionary<ToolStripMenuItem, Command>();

    private void OpenMultiSelect(Command cmd)
    {
        var outputFile = SelectFileDialog(false,cmd.printableName);
        if (File.Exists(outputFile))
        {

            using (MultiInputBoxDialog form = new MultiInputBoxDialog(prompt: cmd.printableName, title: cmd.printableName, cmd.textBoxItems, cmd.lockTextBox))
            {
                if (form.ShowDialog() == DialogResult.OK)
                {
                    var cmds = form.ValueList;
                    string outString = "<<[" + cmd.textValue + ":";
                    if (cmd.textValue.Contains("CUSTOM_SECTION") == true)
                    {
                        outString += "\n";
                    }
                    foreach (var item in cmds)
                    {
                        outString += item;
                        if (cmd.textValue.Contains("CUSTOM_SECTION") == false)
                        {
                            outString += ":";
                        }
                        else
                        {
                            outString += "\n";
                        }
                    }

                    if (cmd.textValue.Contains("CUSTOM_SECTION") == true)
                    {
                        outString += ":";
                    }
                    outString += "]>>\n";
                    var dialogResult = MessageBox.Show("This will included in file:" + outputFile + "\n" + outString + "\n\nWould you like to keep the conents of the old file?", "Generated Configuration", MessageBoxButtons.YesNoCancel);

                    if (dialogResult == DialogResult.OK || dialogResult == DialogResult.Yes)
                    {
                        using (var f = File.AppendText(outputFile))
                        {
                            f.Write(outString);
                        }
                    }
                    else if (dialogResult == DialogResult.No)
                    {
                        try
                        {
                            File.WriteAllText(outputFile, outString);
                        }
                        catch (Exception e)
                        {
                            MessageBox.Show("Something went wrong!\n" + e.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        }
                    }
                }
            }
        }
        else
        {
            MessageBox.Show("Invalid File!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }
    }
    void SelectFileEvent()
    {
        tbDefinitionsFile.Text = SelectFileDialog();
        tbOutputFile.Text = (tbDefinitionsFile.Text.Split(".")).First() + ".h";
    }
    void SelectConfigFileEvent()
    {
        tbConfigDefinitionsFile.Text = SelectFileDialog(true);
    }
    void SaveFileEvent()
    {
        tbOutputFile.Text = SaveFileDialog();
        tbOutputFile.Text = tbOutputFile.Text;
    }
    string SelectFileDialog(bool multiSelect = false, string titleText = "Select File")
    {
        string filePath = "";
        
        using (var fileBrowser = new OpenFileDialog())
        {
            fileBrowser.Title = titleText;
            fileBrowser.Multiselect = multiSelect;
            fileBrowser.FileName = tbDefinitionsFile.Text;
            fileBrowser.RestoreDirectory = true;
            DialogResult result = fileBrowser.ShowDialog();
            
            if (result == DialogResult.OK && !string.IsNullOrWhiteSpace(fileBrowser.FileName) && File.Exists(fileBrowser.FileName))
            {
                if(multiSelect)
                {
                    foreach(var file in fileBrowser.FileNames)
                    {
                        filePath += file;
                        if(file != fileBrowser.FileNames.Last())
                        {
                            filePath += ",";
                        }
                    }
                }
                else
                {
                    filePath = fileBrowser.FileName;
                }
            }
        }


        return filePath;
    }
    string SaveFileDialog()
    {
        string filePath = "";
        
        using (var fileBrowser = new SaveFileDialog())
        {
            fileBrowser.Title = "Create Header File";
            fileBrowser.RestoreDirectory = true;
            fileBrowser.AddExtension = true;
            
            fileBrowser.Filter = "Header|*.h";
            DialogResult result = fileBrowser.ShowDialog();

            if (result == DialogResult.OK && !string.IsNullOrWhiteSpace(fileBrowser.FileName))
            {


                filePath = fileBrowser.FileName;

                if(filePath.EndsWith(".h") == false)
                {
                    filePath += ".h";
                }

            }
        }


        return filePath;
    }
    void OnGenerate()
    {
        string args = "";
        
        if(File.Exists(tbDefinitionsFile.Text) == false || tbOutputFile.Text.Length <= 0)
        {
            MessageBox.Show("Please select definitions file name", "Paths not set",MessageBoxButtons.OK,MessageBoxIcon.Error);
        }
        else
        {
            args += "-f " + tbDefinitionsFile.Text;
            args += " -o " + tbOutputFile.Text;
            if(string.IsNullOrEmpty(authorName) == false && string.IsNullOrWhiteSpace(authorName) == false)
            {
                args += " -a " + (authorName.Replace(" ", "???***&&&$$$"));
            }
            var configFiles = tbConfigDefinitionsFile.Text.Split(',');
            foreach(var opt in configFiles)
            {
                if(File.Exists(opt))
                {
                    args += " -i " + opt;
                }
            }
            RunPyCmd(pythonCmd,   args,pythonPath);
        }
    }
    public Form1()
    {
        InitializeComponent();

        this.authorName = "";
        this.ClientSize = new System.Drawing.Size(750, 160);
        this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
        this.Text = "Hal Builder";

        this.mainDisplay= new System.Windows.Forms.FlowLayoutPanel();
        this.mainDisplay.Size = this.ClientSize;
        this.mainDisplay.AutoSize = true;
        this.mainDisplay.BorderStyle = BorderStyle.FixedSingle;

        var msMenuStrip = new MenuStrip();
        msMenuStrip.Parent = this;


        var exitMenuItem = new ToolStripMenuItem("&Exit", null, (_, _) => Close());

        var filemenuDropdown = new ToolStripMenuItem("File");

        var activeAuthorName = new ToolStripMenuItem("");
        activeAuthorName.Enabled = false;

        void SetAuthorText()
        {
            activeAuthorName.Text = authorName;
        }
        
        var setAuthorName = new ToolStripMenuItem("Set Author");
        setAuthorName.Click += new EventHandler((_, _) => OpenAuthorName());
        setAuthorName.Click += new EventHandler((_, _) => SetAuthorText());

        filemenuDropdown.Click += new EventHandler((_, _) => SetAuthorText());

        var configDropDown = new ToolStripMenuItem("Create Configuration");
        
        cmdTools = new Dictionary<ToolStripMenuItem, Command>();
        
        foreach(var cmd in _commands)
        {
            var dropDownItem = new ToolStripMenuItem(cmd.printableName);
            dropDownItem.Click += new EventHandler((_, _) => OpenMultiSelect(cmd));
            if(cmdTools.TryAdd(dropDownItem, cmd))
            {
                configDropDown.DropDownItems.Add(dropDownItem);
            }
            
        }
        
        filemenuDropdown.DropDownItems.Add(setAuthorName);
        filemenuDropdown.DropDownItems.Add(activeAuthorName);
        filemenuDropdown.DropDownItems.Add(configDropDown);
        filemenuDropdown.DropDownItems.Add(exitMenuItem);
        msMenuStrip.Items.Add(filemenuDropdown);
        msMenuStrip.Items.Add(configDropDown);

        Panel mainPanel = new Panel();
        mainPanel.Size = new Size(750, 200);
        mainPanel.BorderStyle = BorderStyle.FixedSingle;
        mainPanel.BackColor = Color.CadetBlue;
        mainPanel.Parent = this;

        var btnSelectDefinitionsFile = new Button();
        btnSelectDefinitionsFile.Text = "Select Definitions File";
        btnSelectDefinitionsFile.Location = new Point(10,msMenuStrip.Bottom + 10);
        btnSelectDefinitionsFile.Margin = new Padding(5,5,0,5);
        btnSelectDefinitionsFile.Width = 175;
        btnSelectDefinitionsFile.ForeColor = Color.White;
        btnSelectDefinitionsFile.BackColor = Color.Black;

        var btnSelectConfigDefinitionsFile = new Button();
        btnSelectConfigDefinitionsFile.Text = "Select Config Files";
        btnSelectConfigDefinitionsFile.Location = new Point(10,msMenuStrip.Bottom + 40);
        btnSelectConfigDefinitionsFile.Margin = new Padding(5,5,0,5);
        btnSelectConfigDefinitionsFile.Width = 175;
        btnSelectConfigDefinitionsFile.ForeColor = Color.White;
        btnSelectConfigDefinitionsFile.BackColor = Color.Black;

        tbConfigDefinitionsFile = new TextBox();
        tbConfigDefinitionsFile.AcceptsReturn = false;
        tbConfigDefinitionsFile.AcceptsTab = false;
        tbConfigDefinitionsFile.Width = 550;
        tbConfigDefinitionsFile.Height = btnSelectDefinitionsFile.Height-5;
        tbConfigDefinitionsFile.BackColor = TextBox.DefaultBackColor;
        tbConfigDefinitionsFile.Location = new Point(190, msMenuStrip.Bottom + 40);
        tbConfigDefinitionsFile.Text = "...";
        tbConfigDefinitionsFile.Enabled = true;
        tbConfigDefinitionsFile.Click += new EventHandler((_,_) => SelectFileEvent());

        var btnSelectOutputFile = new Button();
        btnSelectOutputFile.Text = "Select Output File";
        btnSelectOutputFile.Location = new Point(10,msMenuStrip.Bottom + 70);
        btnSelectOutputFile.Margin = new Padding(5,5,0,5);
        btnSelectOutputFile.ForeColor = Color.White;
        btnSelectOutputFile.BackColor = Color.Black;
        btnSelectOutputFile.Width = 175;

        var btnGenerateHeader = new Button();
        btnGenerateHeader.Text = "Generate";
        btnGenerateHeader.Location = new Point(10,msMenuStrip.Bottom + 100);
        btnGenerateHeader.Margin = new Padding(5,5,0,5);
        btnGenerateHeader.ForeColor = Color.White;
        btnGenerateHeader.BackColor = Color.Black;
        btnGenerateHeader.Width = 730;
        
        tbDefinitionsFile = new TextBox();
        tbDefinitionsFile.AcceptsReturn = false;
        tbDefinitionsFile.AcceptsTab = false;
        tbDefinitionsFile.Width = 550;
        tbDefinitionsFile.Height = btnSelectDefinitionsFile.Height-5;
        tbDefinitionsFile.BackColor = TextBox.DefaultBackColor;
        tbDefinitionsFile.Location = new Point(190, msMenuStrip.Bottom + 10);
        tbDefinitionsFile.Text = "...";
        tbDefinitionsFile.Enabled = true;
        tbDefinitionsFile.Click += new EventHandler((_,_) => SelectFileEvent());

        tbOutputFile = new TextBox();
        tbOutputFile.AcceptsReturn = false;
        tbOutputFile.AcceptsTab = false;
        tbOutputFile.Width = 550;
        tbOutputFile.Height = btnSelectDefinitionsFile.Height-5;
        tbOutputFile.BackColor = TextBox.DefaultBackColor;
        tbOutputFile.Location = new Point(190, msMenuStrip.Bottom + 70);
        tbOutputFile.Text = "...";
        
        btnSelectDefinitionsFile.Click += new EventHandler((_,_) => SelectFileEvent());
        btnSelectConfigDefinitionsFile.Click += new EventHandler((_,_) => SelectConfigFileEvent());
        btnSelectOutputFile.Click += new EventHandler((_,_) => SaveFileEvent());
        btnGenerateHeader.Click += new EventHandler((_,_) => OnGenerate());

        tbDefinitionsFile.Parent = mainPanel;
        tbConfigDefinitionsFile.Parent = mainPanel;
        tbOutputFile.Parent = mainPanel;
        
        btnGenerateHeader.Parent = mainPanel;
        btnSelectConfigDefinitionsFile.Parent = mainPanel;
        btnSelectOutputFile.Parent = mainPanel;
        btnSelectDefinitionsFile.Parent = mainPanel;




        mainPanel.Parent = this;
    }



}
