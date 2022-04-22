using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using System.IO.Ports;
namespace HyroscoPhciK
{
    public partial class Form1 : Form
    {
        static double[] pidr = new double[100];
        static string GYRO_X;
        static string GYRO_Y;
        static string GYRO_Z;
        static string ACCEL_X;
        static string ACCEL_Y;
        static string ACCEL_Z;
     
   
         int i = 0;

        public delegate void NewShrek();
        public NewShrek myShrek;
        public Form1()
        {
            InitializeComponent();
            string[] portslist = SerialPort.GetPortNames();
            comboBox1.Items.AddRange(portslist);
            myShrek += new NewShrek(Shrekdo);
        }
        public void Shrekdo()
        {
            
            
            textBox1.Text = GYRO_X;
            textBox2.Text = GYRO_Y;
            textBox3.Text = GYRO_Z;
            textBox4.Text = ACCEL_X;
            textBox5.Text = ACCEL_Y;
            textBox6.Text = ACCEL_Z;
            double A = Convert.ToDouble(ACCEL_X);
            double B = Convert.ToDouble(ACCEL_Y);
            double C = Convert.ToDouble(ACCEL_Z);
            pidr[i++] = A;
            if (i >= 100)
            {
                chart1.Series[0].Points.Clear();
                for(int j=0;j<100;j++)
                {
                    chart1.Series[0].Points.AddXY(j, pidr[j]);
                    
                }
                i = 0;
                //chart1.Series[1].Points.AddXY(i++, B);
                //chart1.Series[2].Points.AddXY(i++, C);
                //chart1.Series[0].Points.RemoveAt(0);
             //   chart1.Series[0].Points.AddXY(i++, A);
                //chart1.Series[1].Points.RemoveAt(0);
              //  chart1.Series[2].Points.RemoveAt(0);

            }
            
               // chart1.Series[0].Points.AddXY(i++, A);
            //chart1.Series[1].Points.AddXY(i++, B);
            //chart1.Series[2].Points.AddXY(i++, C);
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            try
            {
                serialPort1.PortName = comboBox1.Text;
                serialPort1.Open();
            }
            catch
            {
                MessageBox.Show("ЕрРОРрр");

            }
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {

        }

        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            byte[] meas = new byte[2];
            serialPort1.Read(meas,0,2);
          //  GYRO_X = Convert.ToString(BitConverter.ToInt16(meas, 0));
          //  GYRO_Y = Convert.ToString(BitConverter.ToInt16(meas, 2));
          //  GYRO_Z = Convert.ToString(BitConverter.ToInt16(meas, 4));
         
            ACCEL_X = Convert.ToString(Math.Round(Convert.ToDouble(BitConverter.ToInt16(meas,0)) / 16383,3));
           // ACCEL_Y = Convert.ToString(Convert.ToDouble(BitConverter.ToInt16(meas, 8)) / 16383);
         //   ACCEL_Z = Convert.ToString(Convert.ToDouble(BitConverter.ToInt16(meas, 10)) / 16383);
            Invoke(myShrek);
        }

        private void textBox3_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox4_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox5_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox6_TextChanged(object sender, EventArgs e)
        {

        }

        private void chart1_Click(object sender, EventArgs e)
        {
            
        }
    }
}
