Imports System.IO.Ports
Public Class Form1
    '宣告委派函式 SetTextCallback,參數型態為字串
    Delegate Sub SetTextCallback(ByVal InputString As String)

    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        For Each PortName As String In SerialPort.GetPortNames
            cmbPort.Items.Add(PortName)
        Next
        cmbPort.SelectedIndex = 0
    End Sub

    Private Sub btnOpenPort_Click(sender As Object, e As EventArgs) Handles btnOpenPort.Click
        If SerialPort1.IsOpen Then
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
            'SerialPort1.ReceivedBytesThreshold = 2
            SerialPort1.Open()
            btnOpenPort.ImageKey = "close"
            btnOpenPort.Text = "關閉"
            btnSend.Enabled = True
        End If
    End Sub

    Private Sub btnSend_Click(sender As Object, e As EventArgs) Handles btnSend.Click
        SerialPort1.Write(txtSend.Text)
    End Sub

    Private Sub SerialPort1_DataReceived(sender As Object, e As SerialDataReceivedEventArgs) Handles SerialPort1.DataReceived
        If e.EventType <> SerialData.Chars Then Exit Sub  '判斷接收的資料是否為字元
        Dim inData As String = SerialPort1.ReadExisting   '取得接收的字串
        DisplayText(inData)     '執行委派函數,執行顯示取得的字串
    End Sub

    Private Sub DisplayText(ByVal comData As String)
        If Me.txtReceive.InvokeRequired Then
            'd是建立的委派物件,傳入的是showString函數指標(主執行緒上的方法函數)
            Dim d As New SetTextCallback(AddressOf showString)
            Me.Invoke(d, New Object() {comData})   '輸入傳日的參數New Object() {comData}
        Else
            showString(comData)
        End If
    End Sub

    Private Sub showString(ByVal comData As String) '主執行緒上的方法函數
        txtReceive.Text = ""
        txtReceive.Text = comData
    End Sub
End Class
