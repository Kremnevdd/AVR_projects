using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;
using System.IO.Ports;

namespace GeneratorPenetrator
{
    public partial class Form1 : Form
    {
        byte[] freq = new byte[4];
        public Form1()
        {
            InitializeComponent();
            string[] ports = SerialPort.GetPortNames();
            comboBox1.Items.AddRange(ports);
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            freq = BitConverter.GetBytes(Convert.ToInt32(textBox1.Text));
            serialPort1.Write(freq, 0, 4);
}

        private void label2_Click(object sender, EventArgs e)
        {

        }


      
        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

       
        private void checkBox1_CheckedChanged_1(object sender, EventArgs e)
        {
            if (checkBox1.Checked == true)
            {
               
                try
                {
                    serialPort1.PortName = comboBox1.Text;
                    serialPort1.Open();
                }
                catch
                {
                    Shrek f = new Shrek();
                    f.ShowDialog();
                    checkBox1.Checked = false;

                }
            }
            else
            {
                serialPort1.Close();
            }
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
    }
}
