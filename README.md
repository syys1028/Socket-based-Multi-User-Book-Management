# Socket-based-Multi-User-Book-Management
💡 [ Protfolio Project 002] 소켓 기반 멀티유저 도서 관리 시스템

## 📌 프로젝트 소개
소켓 기반 멀티유저 도서 관리 시스템은 C 언어로 구현한 네트워크 프로그램으로, 여러 사용자가 동시에 도서 관리 기능을 이용할 수 있는 시스템입니다. 클라이언트-서버 구조를 기반으로, 회원 관리와 도서 목록 관리를 제공합니다. 일반 사용자와 관리자 계정으로 구분되며, 각각의 권한에 맞는 기능을 수행할 수 있습니다.

## 📌 주요 기능
### - 사용자 로그인 및 회원 가입:
사용자는 기존 계정으로 로그인하거나 새로운 계정을 생성할 수 있습니다. 비밀번호는 세 번까지 시도할 수 있으며, 성공적인 로그인 시 도서 관리 기능을 이용할 수 있습니다.

### - 일반 사용자 도서 관리 기능:
  도서 정보 정렬: 책을 식별자 및 가격 순으로 정렬하여 볼 수 있습니다.  
  도서 추가: 새로운 책 정보를 추가할 수 있습니다.  
  도서 수정: 기존의 책 정보를 수정할 수 있습니다.
  도서 삭제: 등록된 책 정보를 삭제할 수 있습니다.
  도서 검색: 제목 또는 저자명으로 책을 검색할 수 있습니다.
  개인 정보 수정: 사용자는 자신의 개인 정보를 수정할 수 있습니다.
### - 관리자 도서 관리 기능:
  도서 정보 정렬: 책을 식별자 및 도서 등록자 순으로 정렬하여 볼 수 있습니다.
  회원 리스트 조회: 현재 등록된 모든 회원 리스트를 볼 수 있습니다.
  회원 관리: 새로운 회원을 추가하고, 기존 회원 정보를 수정 또는 삭제할 수 있습니다.
  도서 관리: 일반 사용자와 동일하게 도서 추가, 수정, 삭제, 검색 기능을 이용할 수 있습니다.
### - 서버 기능:
  다중 클라이언트 연결 지원: 여러 사용자가 동시에 접속하여 서비스를 이용할 수 있습니다.  
  로그 기록: 서버는 사용자 요청을 시간별로 기록하고 처리 상태를 실시간으로 보여줍니다.  
  클라이언트-서버 통신: 클라이언트의 요청에 따라 서버는 도서 및 회원 정보를 읽고 쓰며, 데이터베이스를 관리합니다.

## 📌 구현 상세
### - 클라이언트 (client.c):
  클라이언트는 서버와 연결을 수립하고, 사용자로부터 입력을 받아 서버에 요청을 보냅니다.

메뉴 인터페이스: 사용자는 로그인 후 도서 관리 기능을 이용하거나, 관리자로 로그인하면 회원 관리 기능까지 수행할 수 있습니다.
입출력 관리: 서버로부터 받은 데이터를 처리하여 화면에 출력하고, 사용자의 요청을 전송하는 역할을 합니다.

### - 서버 (server.c):
다중 클라이언트 관리: fork()를 이용해 다중 클라이언트 연결을 지원하며, 각 클라이언트의 요청을 독립적으로 처리합니다.
도서 및 회원 데이터베이스 관리: 서버는 클라이언트로부터 요청을 받아 도서 및 회원 정보를 관리합니다. 이 정보는 파일 기반 데이터베이스에 저장되며, 도서와 회원 정보에 대한 삽입, 수정, 삭제 작업을 처리합니다.
실시간 로그 기록: 클라이언트의 요청을 실시간으로 로그에 기록하여, 시스템의 상태를 파악할 수 있도록 합니다.

### - 도서 관리 (bookprogram.c):
도서 추가: 사용자는 새 책을 데이터베이스에 추가할 수 있습니다.
도서 수정: 등록된 책의 정보를 수정합니다. 일반 사용자는 자신이 등록한 책만 수정할 수 있으며, 관리자는 모든 책을 수정할 수 있습니다.
도서 삭제: 등록된 책을 삭제할 수 있습니다. 관리자는 모든 도서를 삭제할 수 있으며, 일반 사용자는 자신이 등록한 책만 삭제 가능합니다.
정렬 및 검색: 도서 목록을 식별자 또는 가격 순으로 정렬하거나, 제목 및 저자명으로 검색할 수 있습니다.

### - 회원 관리 (memberprogram.c):
회원 추가: 관리자는 새로운 회원을 등록할 수 있습니다.
회원 수정 및 삭제: 관리자는 회원 정보를 수정하거나 삭제할 수 있습니다.
개인 정보 수정: 로그인한 사용자는 자신의 개인 정보를 수정할 수 있습니다.
회원 목록 조회: 관리자는 모든 회원의 목록을 볼 수 있습니다.

## 📌 개발 환경
프로그래밍 언어: C
네트워크 라이브러리: sys/socket.h, netinet/in.h
파일 입출력: fcntl.h, 파일 잠금 기능을 사용하여 데이터의 동시 접근을 관리합니다.
개발 환경: Linux (Ubuntu), GCC, Visual Studio Code
