Imports System.IO.Ports
Public Class Form1
    Private Sub Form1_Load(sender As Object, ByVal e As EventArgs) Handles MyBase.Load
        cmbPort.Items.Clear()
        For Each PortName As String In SerialPort.GetPortNames
            cmbPort.Items.Add(PortName)
        Next
        cmbPort.SelectedIndex = 0
    End Sub

    Private Sub cmbPort_KeyPress(sender As Object, e As KeyPressEventArgs) Handles cmbPort.KeyPress
        e.KeyChar = ControlChars.NullChar
    End Sub

    Private Sub btnOpenPort_Click(sender As Object, ByVal e As EventArgs) Handles btnOpenPort.Click
        If SerialPort1.IsOpen Then
            SerialPort1.Close()
            btnOpenPort.ImageKey = "open"
            btnOpenPort.Text = "開啟"
            btnSend.Enabled = False
            btnReceive.Enabled = False
        Else
            SerialPort1.PortName = cmbPort.SelectedItem.ToString
            SerialPort1.BaudRate = 4800
            SerialPort1.Parity = Parity.None
            SerialPort1.DataBits = 8
            SerialPort1.StopBits = StopBits.One
            SerialPort1.Open()
            btnOpenPort.ImageKey = "close"
            btnOpenPort.Text = "關閉"
            btnReceive.Enabled = True
            btnSend.Enabled = True
        End If
    End Sub

    Private Sub btnSend_Click(sender As Object, e As EventArgs) Handles btnSend.Click
        SerialPort1.Write(txtSend.Text)
    End Sub

    Private Sub btnReceive_Click(sender As Object, e As EventArgs) Handles btnReceive.Click
        txtReceive.Text = SerialPort1.ReadExisting()
    End Sub
End Class

