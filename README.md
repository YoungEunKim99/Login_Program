# Login_Program
TCP/IP 서버-클라이언트 구조의 로그인 프로그램

## Flow chart
<img height="400" alt="flow_chart" src="https://user-images.githubusercontent.com/56907015/153847082-17ab8a4f-b38f-4f45-8222-a599c2687a60.png">

* Client <br>
	* Server의 IP address와 port number를 입력하여 실행
	* Connection 성공 시 "It is connected to Server"" 메시지 출력 실패 시 CONNECTION 끊김
  * ID와 Password 입력 성공 시 CONNECTION 종료/ 실패 시 다시 입력
  * 3번 실패 시 CONNECTION 종료
<br>
* Server <br>
	* 접속을 시도한 Client의 ip 주소가 access.txt에 존재하는 지 확인
	* 존재 시 연결 / 존재하지 않을 시 CONNECTION 거부
  * 입력받은 ID와 Password가 passwd 파일에 존재하면 로그인 성공 메시지 출력
  * 3번 실패 시 CONNECTION 거부



