			<row style="empty" children_layout="default(r=1,c=0,m=3)">
				<label style="prop_title" w="40%" text="Device" />
        <combo_box w="60%" options="0:miniPCIE/USB;1:COM1;2:COM2" value="0" readonly="true" v-data:value="{device}" />
			</row>
			<row style="empty" children_layout="default(r=1,c=0,m=3)">
				<label style="prop_title" w="40%" text="Baud Rate" />
        <combo_box w="60%" options="9600:9600;57600:57600;115200:115200;256000:256000;" value="115200" readonly="true" v-data:value="{baudrate}" />
			</row>
			<row style="empty" children_layout="default(r=1,c=0,m=3)">
				<label style="prop_title" w="40%" text="Data Bits" />
        <combo_box w="60%" options="5:5;6:6;7:7;8:8;" readonly="true" value="8" v-data:value="{databits}" />
			</row>
			<row style="empty" children_layout="default(r=1,c=0,m=3)">
				<label style="prop_title" w="40%" text="Stop Bits" />
        <combo_box w="60%" options="1:1;2:2;" readonly="true" value="1" v-data:value="{stopbits}" />
			</row>
			<row style="empty" children_layout="default(r=1,c=0,m=3)">
				<label style="prop_title" w="40%" text="Parity" />
        <combo_box w="60%" options="0:None;1:Odd;2:Even" value="0" readonly="true" value="0" v-data:value="{parity}" />
			</row>
			<row style="empty" children_layout="default(r=1,c=0,m=3)">
				<label style="prop_title" w="40%" text="RTS HandShake" />
        <combo_box w="60%" options="0:False;1:True;" value="0" readonly="true" value="0" v-data:value="{rts}" />
			</row>
			<row style="empty" children_layout="default(r=1,c=0,m=3)">
				<label style="prop_title" w="40%" text="DTR HandShake" />
        <combo_box w="60%" options="0:False;1:True;" value="0" readonly="true" value="0" v-data:value="{dts}" />
			</row>
