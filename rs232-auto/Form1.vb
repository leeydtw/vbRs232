Imports System.IO.Ports
Public Class Form1
    Dim fstart As Boolean

    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        cmbPort.Items.Clear()
        For Each PortName As String In SerialPort.GetPortNames
            cmbPort.Items.Add(PortName)
        Next
        cmbPort.SelectedIndex = 0
    End Sub

    Private Sub btnOpenPort_Click(sender As Object, e As EventArgs) Handles btnOpenPort.Click
        If SerialPort1.IsOpen Then
            fstart = False
            SerialPort1.Close()
            btnOpenPort.ImageKey = "open"
            btnOpenPort.Text = "開啟"
            btnSend.Enabled = False
        Else
            SerialPort1.PortName = cmbPort.SelectedItem.ToString
            SerialPort1.BaudRate = 9600
            SerialPort1.Parity = Parity.None
            SerialPort1.DataBits = 8
            SerialPort1.StopBits = StopBits.One
            SerialPort1.Open()
            btnOpenPort.ImageKey = "close"
            btnOpenPort.Text = "關閉"
            btnSend.Enabled = True
            fstart = True
        End If
    End Sub

    Private Sub btnSend_Click(sender As Object, e As EventArgs) Handles btnSend.Click
        SerialPort1.Write(txtSend.Text)
    End Sub

    Private Sub Timer1_Tick(sender As Object, e As EventArgs) Handles Timer1.Tick
        If fstart Then
            'txtReceive.Text = ""
            txtReceive.Text += SerialPort1.ReadExisting()
        End If
    End Sub
End Class

