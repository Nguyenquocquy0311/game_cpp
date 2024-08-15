# Game Project

## Giới thiệu chung

- Thực hiện: Nguyễn Quốc Quý
- Mã sinh viên: 20021420
- Lớp INT2215_70
- Tên sản phẩm: Conqueror's Journey
- Link Demo: link

#### Mục lục
1. [Lời nói đầu](#lời-nói-đầu)
2. [Cách chơi game](#cách-chơi-game)
3. [Các thành phần của game](#các-thành-phần-của-game)
4. [Một số hình ảnh của game](#một-số-hình-ảnh-của-game)
5. [Kỹ thuật phát triển](#kỹ-thuật-phát-triển)
6. [Nguồn tham khảo](#nguồn-tham-khảo)
7. [Hướng phát triển](#hướng-phát-triển)

## 1. Lời nói đầu

"Conqueror's Journey" là một trò chơi phiêu lưu hành động đơn giản nhưng thú vị, được phát triển bằng thư viện SDL2. Trong game, người chơi sẽ vào vai một nhân vật chính, chạy qua các chướng ngại vật và thu thập các vật phẩm hỗ trợ để tiến xa hơn. Với đồ họa 2D mộc mạc và âm thanh sống động, trò chơi mang lại cảm giác thú vị và thách thức. Game có nhiều vật phẩm đặc biệt như nhảy cao, nhảy xa và bất tử,..giúp người chơi vượt qua các chướng ngại vật dễ dàng hơn. Bên cạnh đó, "Conqueror's Journey" còn có màn hình menu thân thiện, giúp người chơi dễ dàng bắt đầu hoặc thoát khỏi trò chơi. Hãy thử sức và xem bạn có thể đi được bao xa trong hành trình chinh phục này!

Câu chuyện: John là 1 sĩ quan quân đội cấp cao, sau khi kết thúc nhiệm vụ và được bộ chỉ huy cho nghỉ phép dài ngày, John đã lên kế hoạch đi du lịch Maldivies cùng các đồng đội trong tiểu đội của mình. Tuy nhiên trong lúc đang bay đến Maldivies thì máy bay gặp sự cố và các hành khách trên máy bay buộc phải nhảy dù khẩn cấp để thoát nạn, John vô tình rơi xuống 1 hòn đảo bí ẩn, nằm sâu trong lòng đại dương, nơi sinh sống của 1 bộ tộc hung tợn, bộ tộc này đã tồn tại từ hàng thế kỷ, hoàn toàn tách biệt với thế giới bên ngoài. John, sau khi tiếp đất một cách an toàn, nhận ra rằng mình đang ở trên một hòn đảo hoàn toàn xa lạ, không có dấu hiệu của nền văn minh hiện đại. Hòn đảo được bao phủ bởi những cánh rừng rậm rạp, các vách núi đá hiểm trở, và các tàn tích cổ xưa mang dấu ấn của một nền văn minh đã mất.

Khi bắt đầu cuộc hành trình để tìm kiếm sự trợ giúp, John nhanh chóng nhận ra rằng mình không chỉ phải đối mặt với những thử thách từ thiên nhiên khắc nghiệt, mà còn phải đối mặt với bộ tộc thổ dân bí ẩn, những kẻ coi mọi kẻ ngoại lai là mối đe dọa và luôn trong tư thế sẵn sàng tấn công. Bộ tộc này có những chiến binh cực kỳ dũng mãnh, được trang bị các vũ khí truyền thống và am hiểu mọi ngóc ngách của hòn đảo.

John buộc phải sử dụng tất cả các kỹ năng quân sự của mình để sống sót. Anh phải săn bắn, xây dựng nơi trú ẩn và tránh những cuộc tấn công bất ngờ từ thổ dân. Trong lúc đó, anh cũng cố gắng tìm kiếm và liên lạc với những đồng đội còn sống sót, những người có thể cũng đang lưu lạc đâu đó trên hòn đảo này.

Trong quá trình khám phá hòn đảo, John bắt đầu phát hiện ra những điều kỳ lạ. Những tàn tích cổ đại chứa đựng những bí mật bị chôn vùi, dường như hòn đảo không chỉ là một nơi hoang dã mà còn ẩn chứa một nguồn sức mạnh cổ xưa có thể thay đổi vận mệnh của nhân loại. Bộ tộc thổ dân bảo vệ nguồn sức mạnh này với tất cả sự quyết liệt của mình vì họ biết rằng nếu rơi vào tay kẻ xấu, nó có thể gây ra những hậu quả khôn lường.

Trong cuộc hành trình sinh tồn và khám phá này, John không chỉ phải chiến đấu với bộ tộc thổ dân, mà còn phải đối mặt với những quyết định khó khăn: Liệu anh có nên tiếp tục hành trình tìm kiếm lối thoát khỏi hòn đảo, hay nên cố gắng tìm hiểu và bảo vệ nguồn sức mạnh bí ẩn này? Và liệu John có thể liên lạc được với thế giới bên ngoài trước khi quá muộn, hay sẽ bị mắc kẹt mãi mãi trên hòn đảo đầy bí ẩn này?

Khi câu chuyện tiếp diễn, John sẽ dần dần tìm ra sự thật về hòn đảo, về bộ tộc thổ dân, và về mối liên hệ bí ẩn giữa anh và nguồn sức mạnh cổ xưa kia. Anh sẽ phải đối mặt với những thách thức lớn hơn bao giờ hết, không chỉ để bảo vệ mạng sống của mình, mà còn để quyết định số phận của cả thế giới.

## 2. Cách chơi game
Mở đầu game người chơi có thể lựa chọn chơi game hoặc đọc hướng dẫn về các phím điều khiển Khi người chơi tham gia trò chơi, người chơi phải cố gắng vượt qua nhiều vật cản như tường, cây,..và khi đối đầu với vật cản như kẻ thù, người chơi phải chiến đấu để tiêu diệt. Tốc độ của màn chơi sẽ tăng dần lên theo số điểm và theo độ khó của màn chơi, người chơi cũng có thể ăn các vật phẩm dọc đường. Các vật phẩm này sẽ có những tính năng đặc biệt như đi xuyên vật cản, nhảy cao,.. Khi người chơi chơi hết mạng, game sẽ kết thúc và điểm xuất hiện. Người chơi có thể xem bảng điểm cao.

## 3. Các thành phần của game
- Nhân vật chính
- Vật cản
- Kẻ thù
- Vật phẩm

## 4. Một số hình ảnh của game (dự kiến)

### 1. Ảnh nền các màn chơi

![Game background1](https://static.vecteezy.com/system/resources/previews/009/676/000/non_2x/game-background-cartoon-game-design-nature-asset-vector.jpg)

![Game background2](https://cdna.artstation.com/p/marketplace/presentation_assets/000/246/016/large/file.jpg?1575388524)

![Game background3](https://t3.ftcdn.net/jpg/01/14/47/12/360_F_114471205_0O1mMyKE100dWY4kqoDKBNYJDto53kkt.jpg)

### 2. Ảnh nhân vật chính

![Nhân vật chính](https://i.pinimg.com/originals/1e/d2/05/1ed2051b243b94de885c4c96cbe4b45f.jpg)

### 3. Ảnh menu chính
### 4. Ảnh vật phẩm
- Vật phẩm bất tử

    ![Vật phẩm 1](https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcSoq9sqejTHI1WeN8MRu7xJKK5ZxfJ33VA7OEaCcBsfPiwx04SudC9O3Osgp4PLPb2NRM4&usqp=CAU)
    
- Vật phẩm tăng mạng

    ![Vật phẩm 4](https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcQmk-_EfO07hbyBOx7jj7eygjSodYrZ_AMP9Y_mGlpAyUrPmHCyo7Vf9PQl-yecUB7Dno8&usqp=CAU)

- Vật phẩm nhảy cao

    ![Vật phẩm 2](https://png.pngtree.com/png-vector/20220307/ourmid/pngtree-icons-for-isometric-game-elements-colorful-isolated-vector-illustration-of-item-png-image_4420676.png)

- Vật phẩm nhảy xa

    ![Vật phẩm 3](https://png.pngtree.com/png-vector/20220307/ourmid/pngtree-icons-for-isometric-game-elements-colorful-isolated-vector-illustration-of-item-png-image_4420675.png)
    

## 5. Kỹ thuật phát triển

## 6. Nguồn tham khảo

## 7. Hướng phát triển
