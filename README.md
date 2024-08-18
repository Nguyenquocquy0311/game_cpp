# Conqueror's Journey

## Giới thiệu chung

- **Thực hiện**: Nguyễn Quốc Quý
- **Mã sinh viên**: 20021420
- **Lớp**: INT2215_70
- **Tên sản phẩm**: Conqueror's Journey
- **Chi tiết về sản phẩm**: <https://conquerorsjourney.vercel.app>

#### Mục lục

- [Conqueror's Journey](#conquerors-journey)
  - [Giới thiệu chung](#giới-thiệu-chung)
    - [Mục lục](#mục-lục)
  - [1. Lời nói đầu](#1-lời-nói-đầu)
  - [2. Cách tải game](#2-cách-tải-game)
    - [Cách 1: Không bao gồm code](#cách-1-không-bao-gồm-code)
    - [Cách 2: Bao gồm code và có thể biên dịch](#cách-2-bao-gồm-code-và-có-thể-biên-dịch)
  - [3. Cấu trúc chương trình](#3-cấu-trúc-chương-trình)
  - [4. Kỹ thuật phát triển](#4-kỹ-thuật-phát-triển)
  - [5. Nguồn tham khảo](#5-nguồn-tham-khảo)
  - [6. Hướng phát triển](#6-hướng-phát-triển)

## 1. Lời nói đầu

"Conqueror's Journey" là một trò chơi phiêu lưu hành động đơn giản nhưng thú vị, được phát triển bằng thư viện SDL2. Trong game, người chơi sẽ vào vai một nhân vật chính, chạy qua các chướng ngại vật và thu thập các vật phẩm hỗ trợ để tiến xa hơn. Với đồ họa 2D mộc mạc và âm thanh sống động, trò chơi mang lại cảm giác thú vị và thách thức. Game có nhiều vật phẩm đặc biệt như nhảy cao, nhảy xa và bất tử, giúp người chơi vượt qua các chướng ngại vật dễ dàng hơn. Bên cạnh đó, "Conqueror's Journey" còn có màn hình menu thân thiện, giúp người chơi dễ dàng bắt đầu hoặc thoát khỏi trò chơi. Hãy thử sức và xem bạn có thể đi được bao xa trong hành trình chinh phục này!

## 2. Cách tải game

### Cách 1: Không bao gồm code

- Tải game (được nén thành `.zip`) tại link sau: [Link tải game](https://github.com/Nguyenquocquy0311/game_cpp/releases/tag/v1.0.0)
- **Dung lượng**: ~107MB
- **Hướng dẫn**:
  1. Giải nén file compiler.zip vào một thư mục.
  2. Chạy file `ConquerorsJourney.exe` để bắt đầu chơi.

### Cách 2: Bao gồm code và có thể biên dịch

- **Hướng dẫn**:
  1. **Bước 1**: Clone repo này về máy:

     ```bash
     git clone https://github.com/Nguyenquocquy0311/game_cpp.git
     ```

  2. **Bước 2**: Cài đặt MinGW. Khuyến khích sử dụng phiên bản MinGW-W64 GCC-8.1.0 x86_64-posix-seh.
  3. **Bước 3**: Tải và cài đặt Code::Blocks từ trang web chính thức.
  4. **Bước 4**: Mở Code::Blocks.
  - Chọn File -> New -> Project.
  - Chọn Console Application và nhấn Go.
  - Chọn C++ và nhấn Next. Đặt tên project và chọn đường dẫn đến thư mục bạn đã clone về, sau đó nhấn Next.
  - Đảm bảo đã chọn trình biên dịch MinGW và nhấn Finish.
  5. **Bước 5:** Trong Code::Blocks, chuột phải vào Sources trong Project pane và chọn Add files....
  - Chọn tất cả các file .cpp trong thư mục dự án và nhấn Open.
  - Chuột phải vào Headers trong Project pane và chọn Add files....
  - Chọn tất cả các file .h trong thư mục dự án và nhấn Open.
  6. **Bước 6:** Biên dịch và chạy
  - Chọn Build -> Build để biên dịch project.
  - Sau khi biên dịch thành công, chọn Build -> Run để chạy game.

## 3. Cấu trúc chương trình

Cấu trúc chương trình của "Conqueror's Journey" được tổ chức một cách hợp lý, với các thư mục và file được phân chia rõ ràng:

- **`asset/`**: Thư mục chứa tất cả các tài nguyên của game như hình ảnh, âm thanh và font chữ.
  - **`img/`**: Thư mục chứa hình ảnh cho nhân vật, chướng ngại vật, background và các vật phẩm.
  - **`sound/`**: Thư mục chứa các file âm thanh, bao gồm âm thanh nền, âm thanh bắn đạn, âm thanh va chạm, và các âm thanh hiệu ứng khác.
  - **`fonts/`**: Thư mục chứa font chữ sử dụng trong game.
  
- **`Game.cpp`** và **`Game.h`**: Định nghĩa lớp `Game`, xử lý logic chính của trò chơi như cập nhật, vẽ đồ họa, và xử lý sự kiện.
- **`Player.cpp`** và **`Player.h`**: Định nghĩa lớp `Player`, xử lý hành vi của nhân vật chính.
- **`Enemy.cpp`** và **`Enemy.h`**: Định nghĩa lớp `Enemy`, quản lý các kẻ thù trong game.
- **`Bullet.cpp`** và **`Bullet.h`**: Định nghĩa lớp `Bullet`, quản lý các viên đạn mà nhân vật chính bắn ra.
- **`Obstacle.cpp`** và **`Obstacle.h`**: Định nghĩa lớp `Obstacle`, quản lý các chướng ngại vật trên đường đi.
- **`Item.cpp`** và **`Item.h`**: Định nghĩa lớp `Item`, quản lý các vật phẩm có thể thu thập được trong game.
- **`PowerUp.cpp`** và **`PowerUp.h`**: Định nghĩa lớp `PowerUp`, xử lý các hiệu ứng đặc biệt mà nhân vật có thể nhận được khi thu thập các vật phẩm tăng sức mạnh.

## 4. Kỹ thuật phát triển

"Conqueror's Journey" được phát triển bằng các công nghệ và kỹ thuật sau:

- **Ngôn ngữ lập trình**: C++
- **Thư viện**: SDL2
  - **SDL_image**: Hỗ trợ tải và xử lý hình ảnh trong game.
  - **SDL_mixer**: Hỗ trợ quản lý và phát âm thanh trong game.
  - **SDL_ttf**: Hỗ trợ sử dụng font chữ trong game.
- **IDE**: Code::Blocks
- **MinGW**: Công cụ biên dịch để xây dựng và chạy chương trình.

- Tổ chức mã nguồn với nhiều class, sử dụng vector để quản lý các đối tượng trong game như người chơi, kẻ địch, chướng ngại vật, vật phẩm, và đạn.
- Xử lý va chạm trong game rất chi tiết, bao gồm:
  - Va chạm giữa người chơi với các chướng ngại vật như đá, cây.
  - Va chạm giữa đạn và kẻ địch, đạn và chướng ngại vật.
  - Va chạm giữa người chơi và các vật phẩm thu thập được trên đường đi.
  - Va chạm giữa người chơi và kẻ địch để kiểm tra game over.
- Áp dụng kiến thức toán học để tính toán khoảng cách di chuyển, hướng bắn đạn và điều chỉnh tốc độ di chuyển của nhân vật và kẻ địch.
- Xây dựng menu chính và các màn hình kết thúc game, cho phép người chơi bắt đầu lại hoặc thoát khỏi trò chơi.
- Sử dụng các kỹ thuật đồ họa để vẽ, tạo hiệu ứng, và kết hợp hình ảnh để tạo nên một trải nghiệm game mượt mà và sống động.

## 5. Nguồn tham khảo

- [SDL Documentation](https://wiki.libsdl.org/)
- [Lazy Foo' Productions SDL Tutorials](http://lazyfoo.net/tutorials/SDL/): Một loạt các hướng dẫn về SDL rất hữu ích cho những người mới bắt đầu.
- Sử dụng kỹ thuật chia tách file để tổ chức mã nguồn theo từng module, tham khảo ở kênh youtube [Phát Triển Phần Mềm 123A-Z](https://www.youtube.com/@PhatTrienPhanMem123AZ).
- Các hình ảnh minh họa trong trò chơi được lấy từ nguồn tài nguyên ảnh chất lượng cao của trang [CraftPix](https://craftpix.net).
- Âm thanh trong game được sử dụng từ nguồn miễn phí bản quyền của trang [Mixkit](https://mixkit.co/free-sound-effects/game).

## 6. Hướng phát triển

- **Nâng cấp đồ họa:** Cải thiện hình ảnh và thêm nhiều hiệu ứng hơn để làm cho trò chơi sinh động hơn.
- **Thêm nhiều màn chơi:** Tạo ra nhiều màn chơi với độ khó khác nhau để tăng tính thử thách và sự phong phú cho người chơi.
- **Thêm nhiều item giúp game hấp dẫn hơn:** Các item mới sẽ làm cho trò chơi thêm phần thú vị và cung cấp cho người chơi nhiều tùy chọn cách chơi.
- **Có thể lưu lại lịch sử các lần chơi**: Tính năng lưu lại lịch sử chơi giúp người chơi có thể theo dõi tiến trình của mình.
