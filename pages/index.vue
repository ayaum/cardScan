<template>
  <div>
    <b-table striped :items="data.scanTimes" :fields="fields" />
  </div>
</template>

<script>
import { doc, onSnapshot, getDoc } from 'firebase/firestore'
import dayjs from 'dayjs'
import db from '~/plugins/firebase'
export default {
  name: 'Login',
  data () {
    return {
      data: [],
      fields: ['time', 'id']
    }
  },
  mounted () {
    const day = dayjs().format('YYYYMMDD')
    const docRef = doc(db, 'scanTime', day)
    getDoc(docRef).then((data) => {
      if (data.exists()) {
        onSnapshot(doc(db, 'scanTime', day), (doc) => {
          this.data = doc.data()
        })
      }
    })
  },
  methods: {

  }

}
</script>

<style scoped>
</style>
